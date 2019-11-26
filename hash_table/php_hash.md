### PHP: HashTable
#### 前提
- PHP版本：php7.0.29
- 使用到的文件
  - php-src/Zend/zend_types.h
  - php-src/Zend/zend_hash.h
  - php-src/Zend/zend_hash.c
  - php-src/Zend/zend_string.h
#### 代码分析
##### 数组结构
```
// Bucket: 散列表中存储的元素
typedef struct _Bucket {
	zval              val;  // 存储的具体value，这里嵌入一个zval，而不是一个指针
	zend_ulong        h;    // key 根据times 33 计算得到的哈希值，或者是数值索引编号
	zend_string      *key;  // 存储元素的key

// HashTable 结构
typedef struct _zend_array HashTable;
struct _zend_array {
	zend_refcounted_h gc;
	union {
		struct {
			ZEND_ENDIAN_LOHI_4(
				zend_uchar    flags,
				zend_uchar    nApplyCount,
				zend_uchar    nIteratorsCount,
				zend_uchar    reserve)
		} v;
		uint32_t flags;
	} u;
	uint32_t          nTableMask;   // 哈希值计算掩码，等于nTableSize的负值(nTableMask = -nTableSize)
	Bucket           *arData;   // 存储元素数组，指向第一个Bucket
	uint32_t          nNumUsed; // 已用Bucket数
	uint32_t          nNumOfElements;   // 哈希表有效元素数
	uint32_t          nTableSize;   // 哈希表总大小，为2的n次方
	uint32_t          nInternalPointer;
	zend_long         nNextFreeElement; // 下一个可能的数值索引，如: arr[] = 1; arr["a"] = 2;arr[] = 3; 则nNextFreeElement = 2
	dtor_func_t       pDestructor;
};
```
- HashTable 中有两个非常相近的值: nNumUsed, nNumOfElements
- nNumOfElements  表示哈希值已有元素数，那这个值不跟 nNumUsed一样吗？为什么要定义两个呢？
  - 实际上它们有不同的含义，当将一个元素从哈希表删除时并不会将对应的Bucket移除，而是将Bucket存储的zval修改成IS_UNDEF
  - 只有扩容时发现 nNumOfElements与nNumUsed相差一定数量
  - 这个数量是： ht->nNumUsed - ht->nNumOfElements > (ht->nNumOfElements >> 5) 时才会将已删除的元素全部移除，重新构建哈希表
  - 所以 nNumUsed >= nNumOfElements
- HashTable 中另一个非常重要的值 arData
  - 这个值指向存储元素素组的第一个Bucket, 插入元素时按顺序依次插入数组，比如第一个元素在arData[0], 第二个在arData[1]... arData[nNumUsed]
  - arrData并不是按key映射的散列表，那么映射函数是如何将key与arData中的value建立映射关系的？
    - 插入一个元素时先将元素按先后顺序插入Bucket 数组，位置是idx
    - 再根据key的哈希值映射到散列表中的某个位置nIndex,将idx存入这个位置
    - 查找时先在散列表映射到nIndex，得到value在Bucket数组的位置idx,再从Bucket数组中取出元素
    - 例子
      - $arr["a"] = 1;
      - $arr["b"] = 2;
      - $arr["c"] = 3;
      - $arr["d"] = 4;
      - unset($arr["c"]);
      - ![avatar](images/../../images/php_hash_1.png)
##### HashTable 初始化
```
ZEND_API void ZEND_FASTCALL _zend_hash_init(HashTable *ht, uint32_t nSize, dtor_func_t pDestructor, zend_bool persistent ZEND_FILE_LINE_DC)
{
	GC_REFCOUNT(ht) = 1;
	GC_TYPE_INFO(ht) = IS_ARRAY;
	ht->u.flags = (persistent ? HASH_FLAG_PERSISTENT : 0) | HASH_FLAG_APPLY_PROTECTION | HASH_FLAG_STATIC_KEYS;
	ht->nTableSize = zend_hash_check_size(nSize);   // 初始容量最小为8
	ht->nTableMask = HT_MIN_MASK;
	HT_SET_DATA_ADDR(ht, &uninitialized_bucket);    // 为arrData 分配内存
	ht->nNumUsed = 0;
	ht->nNumOfElements = 0;
	ht->nInternalPointer = HT_INVALID_IDX;
	ht->nNextFreeElement = 0;
	ht->pDestructor = pDestructor;
}
```
##### Hash 算法
````
static zend_always_inline zend_ulong zend_inline_hash_func(const char *str, size_t len)
{
	register zend_ulong hash = Z_UL(5381);

	/* variant with the hash unrolled eight times */
	for (; len >= 8; len -= 8) {
		hash = ((hash << 5) + hash) + *str++;
		hash = ((hash << 5) + hash) + *str++;
		hash = ((hash << 5) + hash) + *str++;
		hash = ((hash << 5) + hash) + *str++;
		hash = ((hash << 5) + hash) + *str++;
		hash = ((hash << 5) + hash) + *str++;
		hash = ((hash << 5) + hash) + *str++;
		hash = ((hash << 5) + hash) + *str++;
	}
	switch (len) {
		case 7: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
		case 6: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
		case 5: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
		case 4: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
		case 3: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
		case 2: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
		case 1: hash = ((hash << 5) + hash) + *str++; break;
		case 0: break;
EMPTY_SWITCH_DEFAULT_CASE()
	}

	/* Hash value can't be zero, so we always set the high bit */
#if SIZEOF_ZEND_LONG == 8
	return hash | Z_UL(0x8000000000000000);
#elif SIZEOF_ZEND_LONG == 4
	return hash | Z_UL(0x80000000);
#else
# error "Unknown SIZEOF_ZEND_LONG"
#endif
}
````
- PHP的Hash采用的是目前最为普遍的DJBX33A (Daniel J. Bernstein, Times 33 with Addition)
- 算法的核心思想就是:
  -  hash(i) = hash(i-1) * 33 + str[i]
- PHP中并没有使用直接乘33, 而是采用了: hash << 5 + hash
##### 映射函数
- 映射函数(散列函数)是散列表的关键部分，它将key 与 value 建立映射关系,一般映射函数可以根据key的哈希值与Bucket数组大小取模得到
  - 即 key->h % ht->nTableSize
  - PHP的做法： nIndex = key->h | nTablesMask
- nTablesMask  为 nTableSize 的负数，即：nTablesMask = -nTableSize 
- 因为 nTableSize 等于 2 ^ n, 所以nTablesMask二进制右侧全部为0，也就保证了nIndex 落在数组索引的范围之内(|nIndex| <= nTableSize)
  - 11111111 11111111 11111111 11111000 -8
  - 11111111 11111111 11111111 11110000 -16
  - 11111111 11111111 11111111 11100000 -32
  - 11111111 11111111 11111111 11000000 -64
  - 11111111 11111111 11111111 10000000 -64
##### 参考资料
- 《PHP7内核剖析》
- [PHP中的Hash算法](http://www.laruence.com/2009/07/23/994.html) 
