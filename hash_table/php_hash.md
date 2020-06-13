### PHP: HashTable
#### 前提
- PHP版本：php7.0.29
- 使用到的文件
  - php-src/Zend/zend_types.h
  - php-src/Zend/zend_hash.h
  - php-src/Zend/zend_hash.c
  - php-src/Zend/zend_string.h
#### 代码分析
##### 基本结构
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
###### bucket结构分析
- val 参数
  - 对应HashTable设计中的value。始终是zval类型
  - zval结构
	```
	struct _zval_struct {
		zend_value        value;			/* 表明zval类型 */
		union {
			struct {
				ZEND_ENDIAN_LOHI_4(
					zend_uchar    type,			/* active type */
					zend_uchar    type_flags,
					zend_uchar    const_flags,
					zend_uchar    reserved)	    /* call info for EX(This) */
			} v;
			uint32_t type_info;
		} u1;
		union {
			uint32_t     var_flags;
			uint32_t     next;                 /* 用来解决哈希冲突 */
			uint32_t     cache_slot;           /* 运行时缓存 */
			uint32_t     lineno;               /* 对于zend_ast_zval存行号 */
			uint32_t     num_args;             /* EX(This) 参数个数*/
			uint32_t     fe_pos;               /* foreach 位置 */
			uint32_t     fe_iter_idx;          /* foreach 游标的标记 */
		} u2;
	};
	```
  - 每个zval类型16个字节
- h 参数
  - 对应HashTable设计中的h，表示数字key或者字符串key的h值
- key 参数
  - 对应HashTable设计中的key，表示字符串的key
  - zend_string结构
	```
	struct _zend_string {
		zend_refcounted_h gc;	/* 8字节，内嵌的gc，引用计数及字符串类别存储*/
		zend_ulong        h;   /* hash值，8字节，字符串的哈希值 */
		size_t            len;	/* 8字节，字符串的长度*/
		char              val[1]; /*柔性数数组，占1位，字符串的值存粗的位置*/
	};

	typedef struct _zend_refcounted_h {
		uint32_t         refcount;			/* gc,整块占用8字节 */
		union { // 4字节
			struct {
				ZEND_ENDIAN_LOHI_3(
					zend_uchar    type,	/* 等同于zval的u1.v.type*/
					zend_uchar    flags,    /* 字符串的类型数据 */
					uint16_t      gc_info)  /* 垃圾回收标记颜色用 */
			} v;
			uint32_t type_info;
		} u;
	} zend_refcounted_h;
	```
	- zend_string是一种带有字符串长度，h值，gc信息的字符串数组的包装，提升了性能和空间效率
- bucket 的类型
  - 未使用bucket
    - 最初所有的bucket都是未使用的状态
  - 有效bucket
    - 存储着有效的数据(key, val, h),当进行插入时，会选择一个未使用bucket，这样该bucket就变成了有效bucket
    - 更新操作只能发生在有效bucket上，更新后，仍然是有效bucket
  - 无效bucket
    - 当bucket上存储的数据被删除时，有效bucket就会变成无效bucket
- bucket类型间的转换
  - 在内存分布上有效bucket和无效bucket会交替分布，但都在未使用bucket的前面
  - 插入的时候永远在未使用bucket上进行
  - 当由于删除等操作，导致无效bucket非常多，而有效bucket很少时，会对整个bucket数组进行rehash操作，这样，稀疏的有效bucket就会变得连续和紧密
  - 部分无效bucket会被重新利用而变为有效bucket
  - 还有一部分有效bucket和无效bucket会被释放出来，重新变成未使用bucket
  - ![avatar](images/../../images/php_hash_2.png)

###### HashTable结构分析
- gc 参数
  - 引用计数相关 
  - 在PHP7中，引用计数不再是zval的字段，而是被设计在zval的value字段所指向的结构体中
- argData 参数
  - 实际的存储容器
  - 通过指针指向一段连续的内存，存储着bucket数组
- nTableSize 参数
  - HashTable 的大小
  - 表示 arData指向的bucket数组的大小，即所有bucket的数量
  - 最小值未8
  - 最大值在32位系统中是0x40000000(2 ^ 30)，在64位系统中0x80000000(2 ^ 31)
- nNumUsed 参数
  - 指向已使用bucket的数量，包括有效bucket和无效bucket的数量
  - 在bucket数组中，下标从0 ~ (nNumUsed - 1)的bucket都属于已使用bucket
  - 而下标为nNumUsed ~ (nTableSize - 1)的bucket都属于未使用bucket
- nNumOfElements 参数
  - 有效bucket的数量
  - 该值总是小于或等于nNumUsed
- nTableMask 参数
  - 掩码。一般为-nTableSize
- nInternalPointer 参数
  - HashTable的全局默认游标
- nNextFreeElement 参数
  - HashTable的自然key
  - 自然key是指HashTable的应用语义是纯数组时，插入元素无须指定key，key会以nNextFreeElement的值为准
  - 例如
    - 该字段初始值为0
    - $a[] = 1，实际上是插入到key等于0的bucket上
    - 然后nNextFreeElement会递增1，代表下一个自然插入的元素的key是1
- pDestructor 参数
  - 析构函数
  - 当bucket元素被更新或被删除时，会对bucket的value调用该函数
  - 如果value是引用计数的类型，那么会对value应用计数减1，进而引发可能的gc
- u 联合体
  - 占用4个字节。可以存储一个uint32_t类型的flags
  - 也可以存储由4个unsigned char 组成的结构体v
  - u.v.flags 参数
    - 用各个bit来表达HashTable的各种标记
    - 共有下面6中flag，分别对应 u.v.flags的第1位到6位
		```
		#define HASH_FLAG_PERSISTENT       (1<<0)	// 是否使用持久化内存（不使用内存池）
		#define HASH_FLAG_APPLY_PROTECTION (1<<1)	// 是否开启递归遍历保护
		#define HASH_FLAG_PACKED           (1<<2)	// 是否是packed array
		#define HASH_FLAG_INITIALIZED      (1<<3)	// 是否初始化
		#define HASH_FLAG_STATIC_KEYS      (1<<4)	// 标记HashTable的Key是否为long key
		#define HASH_FLAG_HAS_EMPTY_IND    (1<<5)	// 是否存在空的间接val
		```
  - u.v.nApplyCount 参数
    - 递归遍历计数
    - 为了解决循环引用导致的死循环问题
    - 当对某个数组进行某种递归操作时，在递归调入栈之前将nApplyCount加1，递归调出栈之后将nApplyCount减1
    - 当循环引用出现时，递归调用会不断入栈，当nApplyCount增加到一定阀值时，不再继续递归下去，返回一个合法的值，并打印"recursion detected"之类的warning或者error日志
    - 这个阀值一般不大于3
  - u.v.nIteratorsCount 参数
    - 迭代器计数
    - PHP中每一个foreach语句都会在全局变量EG中创建一个迭代器
    - 迭代器包含正在遍历的HashTable和游标信息
    - 该字段记录了runtime正在迭代当前的HashTable的迭代器的数量
  - u.v.consistency 参数
    - 成员用于调试目的
		```
		#define HT_OK					0x00	// 正常状态，各种数据完全一致
		#define HT_IS_DESTROYING		0x40	// 正在删除所有的内容，包括arBuckets本身
		#define HT_DESTROYED			0x80	// 已删除，包括arBuckets本身
		#define HT_CLEANING				0xc0	// 正在清除所有的arBuckets执行的内容，但不包括arBuckets本身
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
- 《PHP7底层设计和源码实现》
- 《PHP7内核剖析》
- [PHP中的Hash算法](http://www.laruence.com/2009/07/23/994.html) 
