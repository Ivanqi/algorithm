#### PHP7数组排序源码
##### 前提
- PHP版本：php7.0.29
- 使用到的文件
  - php-src/Zend/zend_sort.c
  - php-src/ext/standard/array.c
  - php-src/Zend/zend_hash.c
  - php-src/Zend/zend_hash.h
##### 代码分析
- 通过搜索 PHP_FUNCTION(sort)
    ```
    PHP_FUNCTION(sort)
    {
        zval *array;
        zend_long sort_type = PHP_SORT_REGULAR;
        compare_func_t cmp;

        if (zend_parse_parameters(ZEND_NUM_ARGS(), "a/|l", &array, &sort_type) == FAILURE) {
            RETURN_FALSE;
        }

        cmp = php_get_data_compare_func(sort_type, 0);

        if (zend_hash_sort(Z_ARRVAL_P(array), cmp, 1) == FAILURE) {
            RETURN_FALSE;
        }
        RETURN_TRUE;
    }
    ```
- php_get_data_compare_func 函数
  - sort_type = PHP_SORT_REGULAR
  - reverse = 0
  - 返回 php_array_data_compare 函数，用于比较两个值的大小
    ```
    /**
        这个函数中的数字总是小于字符串，因为比较两种不同的数据类型没有多大意义。
        先判断first 和 second 的类型，再进行各种分支比较。都是字符串的情况下,会使用C的memcmp 比较两个字符串前N个字节。这个N是字符串小的那个
    */
    static int php_array_data_compare(const void *a, const void *b) /* {{{ */
    {
        Bucket *f;
        Bucket *s;
        zval result;
        zval *first;
        zval *second;

        f = (Bucket *) a;
        s = (Bucket *) b;

        first = &f->val;
        second = &s->val;

        if (UNEXPECTED(Z_TYPE_P(first) == IS_INDIRECT)) {
            first = Z_INDIRECT_P(first);
        }
        if (UNEXPECTED(Z_TYPE_P(second) == IS_INDIRECT)) {
            second = Z_INDIRECT_P(second);
        }
        if (compare_function(&result, first, second) == FAILURE) {
            return 0;
        }

        ZEND_ASSERT(Z_TYPE(result) == IS_LONG);
        return Z_LVAL(result);
    }
    ```
- 通过 zend_hash.h 寻找到 zend_hash_sort_ex 函数
    ```
    #define zend_hash_sort(ht, compare_func, renumber) \
        zend_hash_sort_ex(ht, zend_sort, compare_func, renumber)
    ```
    ```
    ZEND_API int ZEND_FASTCALL zend_hash_sort_ex(HashTable *ht, sort_func_t sort, compare_func_t compar, zend_bool renumber)
    {
        Bucket *p;
        uint32_t i, j;

        IS_CONSISTENT(ht);
        HT_ASSERT(GC_REFCOUNT(ht) == 1);

        if (!(ht->nNumOfElements>1) && !(renumber && ht->nNumOfElements>0)) { /* Doesn't require sorting */
            return SUCCESS;
        }

        if (ht->nNumUsed == ht->nNumOfElements) {
            i = ht->nNumUsed;
        } else {
            for (j = 0, i = 0; j < ht->nNumUsed; j++) {
                p = ht->arData + j;
                if (UNEXPECTED(Z_TYPE(p->val) == IS_UNDEF)) continue;
                if (i != j) {
                    ht->arData[i] = *p;
                }
                i++;
            }
        }
        # sort 为上面传入的函数，可以跳转到 zend_sort
        sort((void *)ht->arData, i, sizeof(Bucket), compar,
                (swap_func_t)(renumber? zend_hash_bucket_renum_swap :
                    ((ht->u.flags & HASH_FLAG_PACKED) ? zend_hash_bucket_packed_swap : zend_hash_bucket_swap)));
        # 后面是根据renumber判断是否需要重排索引内存回收等操作先省略了 
    ```
- 通过 zend_sort.c 文件可以找到 zend_sort 函数
    ```
    ZEND_API void zend_sort(void *base, size_t nmemb, size_t siz, compare_func_t cmp, swap_func_t swp)
    {
        while (1) {
            if (nmemb <= 16) {
                zend_insert_sort(base, nmemb, siz, cmp, swp);
                return;
            } else {
                char *i, *j;
                char *start = (char *)base;
                char *end = start + (nmemb * siz);
                size_t offset = (nmemb >> Z_L(1));
                char *pivot = start + (offset * siz);

                if ((nmemb >> Z_L(10))) {
                    size_t delta = (offset >> Z_L(1)) * siz;
                    zend_sort_5(start, start + delta, pivot, pivot + delta, end - siz, cmp, swp);
                } else {
                    zend_sort_3(start, pivot, end - siz, cmp, swp);
                }
                swp(start + siz, pivot);
                pivot = start + siz;
                i = pivot + siz;
                j = end - siz;
                while (1) {
                    while (cmp(pivot, i) > 0) {
                        i += siz;
                        if (UNEXPECTED(i == j)) {
                            goto done;
                        }
                    }
                    j -= siz;
                    if (UNEXPECTED(j == i)) {
                        goto done;
                    }
                    while (cmp(j, pivot) > 0) {
                        j -= siz;
                        if (UNEXPECTED(j == i)) {
                            goto done;
                        }
                    }
                    swp(i, j);
                    i += siz;
                    if (UNEXPECTED(i == j)) {
                        goto done;
                    }
                }
    done:
                swp(pivot, i - siz);
                if ((i - siz) - start < end - i) {
                    zend_sort(start, (i - start)/siz - 1, siz, cmp, swp);
                    base = i;
                    nmemb = (end - i)/siz;
                } else {
                    zend_sort(i, (end - i)/siz, siz, cmp, swp);
                    nmemb = (i - start)/siz - 1;
                }
            }
        }
    }
    ```
    - 这里的排序算法，在元素长度小于等于16的时候，会使用插入排序
    - 元素算法大于16的时候，使用快速排序
    - pivot 的构成
        ```
        char *start = (char *)base;
        char *end = start + (nmemb * siz);
        size_t offset = (nmemb >> Z_L(1));
        char *pivot = start + (offset * siz);
        ...
        swp(start + siz, pivot);
        pivot = start + siz;
        ```
        - 这里貌似是三数取中法的变种。现在还不能很好解答
- 插入排序
    ```
    ZEND_API void zend_insert_sort(void *base, size_t nmemb, size_t siz, compare_func_t cmp, swap_func_t swp) /* {{{ */{
        switch (nmemb) {
            case 0:
            case 1:
                break;
            case 2:
                zend_sort_2(base, (char *)base + siz, cmp, swp);
                break;
            case 3:
                zend_sort_3(base, (char *)base + siz, (char *)base + siz + siz, cmp, swp);
                break;
            case 4:
                {
                    size_t siz2 = siz + siz;
                    zend_sort_4(base, (char *)base + siz, (char *)base + siz2, (char *)base + siz + siz2, cmp, swp);
                }
                break;
            case 5:
                {
                    size_t siz2 = siz + siz;
                    zend_sort_5(base, (char *)base + siz, (char *)base + siz2, (char *)base + siz + siz2, (char *)base + siz2 + siz2, cmp, swp);
                }
                break;
            default:
                {
                    char *i, *j, *k;
                    char *start = (char *)base;
                    char *end = start + (nmemb * siz);
                    size_t siz2= siz + siz;
                    char *sentry = start + (6 * siz);
                    for (i = start + siz; i < sentry; i += siz) {
                        j = i - siz;
                        if (!(cmp(j, i) > 0)) {
                            continue;
                        }
                        while (j != start) {
                            j -= siz;
                            if (!(cmp(j, i) > 0)) {
                                j += siz;
                                break;
                            }
                        }
                        for (k = i; k > j; k -= siz) {
                            swp(k, k - siz);
                        }
                    }
                    for (i = sentry; i < end; i += siz) {
                        j = i - siz;
                        if (!(cmp(j, i) > 0)) {
                            continue;
                        }
                        do {
                            j -= siz2;
                            if (!(cmp(j, i) > 0)) {
                                j += siz;
                                if (!(cmp(j, i) > 0)) {
                                    j += siz;
                                }
                                break;
                            }
                            if (j == start) {
                                break;
                            }
                            if (j == start + siz) {
                                j -= siz;
                                if (cmp(i, j) > 0) {
                                    j += siz;
                                }
                                break;
                            }
                        } while (1);
                        for (k = i; k > j; k -= siz) {
                            swp(k, k - siz);
                        }
                    }
                }
                break;
        }
    }
    ```
    - 元素长度小于等于5的时候
      - 直接通过 if else 嵌套判断排序。比如 zend_sort_2，zend_sort_3，zend_sort_4， zend_sort_5这些函数
      - zend_sort_x 里面 cmp 这个函数，当 sort_flags 为 SORT_REGULAR 时 sort 函数的 cmp 调用的是 array.c 中的下面这个函数，返回值分成 小于0(b>1), 0(b==a), 大于0(a>b)对比失败也是0
      - 然后再通过swap函数对调两个变量的内存地址
    - 元素长度大于5
      - 通过定义哨兵 char *sentry = start + (6 * siz);, 把函数拆成两边部分，再通过插入排序