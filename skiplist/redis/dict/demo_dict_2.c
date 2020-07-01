#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>

#include "demo_dict_1.h"

static int dict_cat_resize = 1;
static unsigned int dict_force_resize_ratio = 5;


static int _dictExpandIfNeeded(dict *t);
static unsigned long _dictNextPower(unsigned long size);
static int _dictKeyIndex(dict *ht, const void *key);
static int _dictInit(dict *ht, dictType *type, void *privDatePtr);

// Thomas Wang's 32 bit Mix Function
unsigned int dictIntHashFuncion(unsigned int key) {

    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

unsigned int dictIdentiyHashFunction(unsigned int key) {

    return key;
}

static uint32_t dict_hash_function_seed = 5381;

void dictSetHashFunctionSeed(uint32_t seed) {

    dict_hash_function_seed = seed;
}

uint32_t dictGetHashFunctionSeed(void) {

    return dict_hash_function_seed;
}

/* MurmurHash2, by Austin Appleby
 * Note - This code makes a few assumptions about how your machine behaves -
 * 1. We can read a 4-byte value from any address without crashing
 * 2. sizeof(int) == 4
 *
 * And it has a few limitations -
 *
 * 1. It will not work incrementally.
 * 2. It will not produce the same results on little-endian and big-endian
 *    machines.
 *
 * 算法的具体信息可以参考 http://code.google.com/p/smhasher/
 */
unsigned int dictGenHashFunction(const void *key, int len) {

    uint32_t seed = dict_hash_function_seed;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    uint32_t h = seed ^ len;

    const unsigned char *data = (const unsigned char *)key;

    while (len >= 4) {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0]; h *= m; 
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int) h;
}

unsigned int dictGenCaseHashFunction(const unsigned char *buf, int len) {

    unsigned int hash = (unsigned int) dict_hash_function_seed;

    while (len--) {
        hash = ((hash << 5) + hash) + (tolower(*buf++));    // hash * 33 + c
    }
    return hash;
}

// 重置哈希表的各项属性
static void _dictReset(dictht *ht) {

    ht->table = NULL;
    ht->size = 0;
    ht->sizemark = 0;
    ht->used = 0;
}

// 创建一个新字典
dict *dictCreate(dictType *type, void *privDataPtr) {

    // 分配空间
    dict *d = malloc(sizeof(*d));

    // 初始化字典
    _dictInit(d, type, privDataPtr);

    return d;
}

// 初始化字典
int _dictInit(dict *d, dictType *type, void *privDataPtr) {

    // 初始化 ht[0]
    _dictReset(&d->ht[0]);

    // 初始化 ht[1]
    _dictReset(&d->ht[1]);

    // 初始化字典属性
    d->type = type;
    d->privdate = privDataPtr;
    d->rehashindex = -1;
    d->iterators = 0;

    return DICT_OK;
}

// 对字典进行紧缩，让节点数/桶数的比率接近 <= 1
int dictResize(dict *d) {

    int minimal;
    
    // 不能在 dict_can_resize 为假
    // 或者字典正在 rehash时调用
    if (!dict_cat_resize || dictIsRehashing(d)) return DICT_ERR;

    minimal = d->ht[0].used;

    if (minimal < DICT_HT_INITAL_SIZE) {
        minimal = DICT_HT_INITAL_SIZE;
    }

    return dictExpand(d, minimal);
}

/**
 * 创建一个新哈希表，并视情况，进行以下动作之一
 *  1. 如果字典里的ht[0]为空，将新哈希表赋值给它
 *  2. 如果字典里的ht[0]不为空，那么将新哈希表赋值给ht[1],并打开rehash标识
 */
int dictExpand(dict *d, unsigned long size) {

    dictht n;   // 新的哈希表

    // 计算哈希表的真实大小
    unsigned long realsize = _dictNextPower(size);

    if (dictIsRehashing(d) || d->ht[0].used > size) {
        return DICT_ERR;
    }

    // 创建并初始化新哈希表
    n.size = realsize;
    n.sizemark = realsize - 1;
    n.table = malloc(realsize * sizeof(dictEntry*));
    n.used = 0;

    // 如果ht[0]为空，那么这就是一次创建新哈希表行为
    // 将新哈希表设置为 ht[0], 然后返回
    if (d->ht[0].table == NULL) {
        d->ht[0] = n;
        return DICT_OK;
    }

    // 如果ht[0]不为空， 那么这就是一次扩展字典的行为
    // 将新哈希表设置为ht[1]， 并打开rehash标识
    d->ht[1] = n;
    d->rehashindex = 0;
    return DICT_OK;
}

/**
 * 执行 N 步渐进式 rehash
 * 
 * 如果执行之后哈希表还有元素需要rehash， 那么返回-1
 * 如果哈希表里面所有元素已经迁移完毕，那么返回0
 * 
 * 每步 rehash 都会移动哈希表数组内某个索引上的整个链表节点
 * 所有从 ht[0] 迁移都 ht[1] 的key 可能不止一个
 */
int dictRehash(dict *d, int n) {

    if (!dictIsRehashing(d)) return 0;

    while (n--) {
        dictEntry *de, *nextde;

        // 如果 ht[0]已经为空，那么迁移完毕
        // 用 ht[1] 代替原有的 ht[0]
        if (d->ht[0].used == 0) {

            // 释放 ht[0]的哈希表数组
            free(d->ht[0].table);

            // 将ht[0]指向ht[1]
            d->ht[0] = d->ht[1];

            // 清空ht[1]的指针
            _dictReset(&d->ht[1]);

            // 关闭 rehash标识
            d->rehashindex = -1;

            // 通知调用者， rehash完毕
            return 0;
        }

        assert(d->ht[0].size > (unsigned) d->rehashindex);

        // 移动到数组中首个不为NULL链表的索引上
        while (d->ht[0].table[d->rehashindex] == NULL) d->rehashindex++;

        // 指向链表
        de = d->ht[0].table[d->rehashindex];

        /**
         * 将链表内的所有元素从ht[0]迁移到ht[1]
         * 因为桶内的元素通常只有一个，或者不多于某个特定比率
         * 所有可以将这个操作看作O(1)
         */
        while (de) {
            unsigned int h;

            nextde = de->next;

            // 计算元素在ht[1]的哈希值
            h = dictHashKey(d, de->key) & d->ht[1].sizemark;

            // 添加节点到 ht[1]，调整指针
            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;

            // 更新计数器
            d->ht[0].used--;
            d->ht[1].used++;

            de = nextde;
        }

        // 设置指针为NULL， 方便下次rehash时跳过
        d->ht[0].table[d->rehashindex] = NULL;

        // 前进至下一索引
        d->rehashindex++;
    }

    // 通知调用者，还是元素等待 rehash
    return 1;
}

/**
 * 以毫秒为单位，返回当时时间
 */
long long timeInMilliseconds(void) {

    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

/**
 * 在给定毫秒数内，以100步为单位，对字典进行rehash
 * T = O(N)， N为被 rehash的 key-value 对数量
 */
int dictRehashMilliseconds(dict *d, int ms) {

    long long start = timeInMilliseconds();
    int rehashs = 0;

    while (dictRehash(d, 100)) {
        rehashs += 100;
        if (timeInMilliseconds() - start > ms) break;
    }

    return rehashs;
}

/**
 * 如果允许条件的话，将一个元素从ht[0]迁移至ht[1]
 * 
 * 这个函数被其他查找和更新函数所调用，从而实现渐进式 rehash
 */
static void _dictRehashStep(dict *d) {

    // 只在没有安全迭代器的时候，才能进行迁移
    // 否则可能产生重复元素，或者丢失元素
    if (d->iterators == 0) {
        dictRehash(d, 1);
    }
}

// 添加给定 key-value对 到字典
int dictAdd(dict *d, void *key, void *val) {

    // 添加key到哈希表，返回包含该key节点
    dictEntry *entry = dictAddRaw(d, key);

    // 添加失败
    if (!entry) return DICT_ERR;

    // 设置节点的值
    dictSetVal(d, entry, val);

    return DICT_OK;
}

/**
 * 添加key到字典的底层实现， 完成之后返回新节点
 * 如果key已经存在，返回NULL
 */
dictEntry *dictAddRaw(dict *d, void *key) {

    int index;
    dictEntry *entry;
    dictht *ht;

    // 尝试渐进式地 rehash 一个元素
    if (dictIsRehashing(d)) _dictRehashStep(d);

    // 查找可容纳新元素的索引位置
    // 如果元素已存在，index为-1
    if ((index = _dictKeyIndex(d, key)) == -1) {
        return NULL;
    }

    // 决定该把新元素放在那个哈希表
    ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0];

    // 为新元素分配节点空间
    entry = malloc(sizeof(*entry));

    // 新节点的后继指针指向旧的表头节点
    entry->next = ht->table[index];

    // 设置新节点为表头
    ht->table[index] = entry;

    // 更新已有节点数量
    ht->used++;

    // 关联起节点和key
    dictSetKey(d, entry, key);

    // 返回新节点
    return entry;
}

/***
 * 用新值代替key原有的值
 * 
 * 如果key 不存在，将关联添加到哈希表中
 * 
 * 如果关联是新创建的，返回1，如果关联是被更新的，返回0
 */
int dictReplace(dict *d, void *key, void *val) {

    dictEntry *entry, auxentry;

    /**
     * 尝试添加新元素到哈希表
     * 只要key 不存在，添加就会成功
     * O(1)
     */
    if (dictAdd(d, key, val) == DICT_OK) {
        return 1;
    }

    /**
     * 如果添加失败，那么说明元素已经存在
     * 获取这个元素对应的节点
     * O(1)
     */
    entry = dictFind(d, key);

    auxentry = *entry;          // 指向旧值
    dictSetVal(d, entry, val);  // 设置新值
    dictFreeVal(d, &auxentry);  // 释放旧值

    return 0;
}

/**
 * 类似于 dictAddRaw()
 * dictReplaceRaw 无论在新添加节点还是更新节点的情况下
 * 都返回key所对应的节点
 */
dictEntry *dictReplaceRaw(dict *d, void *key) {

    // 查找
    dictEntry *entry = dictFind(d, key);

    // 没找到就添加，找到直接返回
    return entry ? entry : dictAddRaw(d, key);
}

// 按key 查找并删除节点
static int dictGenericDelete(dict *d, const void *key, int nofree) {

    unsigned int h, idx;
    dictEntry *he, *prevHe;
    int table;

    // 空表?
    if (d->ht[0].size == 0) return DICT_ERR;

    // 渐进式 rehash
    if (dictIsRehashing(d)) _dictRehashStep(d);

    // 计算哈希值
    h = dictHashKey(d, key);

    // 在两个哈希表中查找
    for (table = 0; table <= 1; table++) {
        // 索引值
        idx = h & d->ht[table].sizemark;
        // 索引在数组中对应的表头
        he = d->ht[table].table[idx];
        prevHe = NULL;

        /**
         * 遍历链表
         * 因为链表的元素数量通常为1，或者维持在一个很小的比率
         * 因此可以将这个操作看作O(1)
         */
        while (he) {
            // 对比
            if (dictCompareKeys(d, key, he->key)) {
                if (prevHe) {
                    prevHe->next = he->next;
                } else {
                    d->ht[table].table[idx] = he->next;
                }

                // 释放节点的键和值
                if (!nofree) {
                    dictFreeKey(d, he);
                    dictFreeVal(d, he);
                }

                // 释放节点
                free(he);

                d->ht[table].used--;

                return DICT_OK;
            }
            prevHe = he;
            he = he->next;
        }

        /**
         * 如果不是正在进行 rehash
         * 那么无须遍历ht[1]
         */
        if (!dictIsRehashing(d)) break;
    }

    return DICT_ERR;    // 没有找到
}

// 删除哈希表中的key，并且释放保存这个key的节点
int dictDelete(dict *ht, const void *key) {

    return dictGenericDelete(ht,key,0);
}

// 删除哈希表中的key， 但是并不释放保持这个 key 的节点
int dictDeleteNoFree(dict *ht, const void key) {

    return dictGenericDelete(ht, key, 1);
}

// 销毁给定哈希表
int _dictClear(dict *d, dictht *ht) {

    unsigned long i;

    // 遍历哈希表数组
    for (i = 0; i < ht->size && ht->used > 0; i++) {
        dictEntry *he, *nextHe;

        if ((he = ht->table[i]) == NULL) continue;

        /**
         * 释放整个链表上的元素
         * 因为链表的元素数量通常为1，或者维持在一个很小的比率
         * 因此可以将这个操作看作O(1)
         */
        while (he) {
            nextHe = he->next;

            dictFreeKey(d, he);
            dictFreeVal(d, he);

            free(he);

            ht->used--;

            he = nextHe;
        }
    }

    free(ht->table);

    _dictReset(ht);

    return DICT_OK;
}

// 清空并释放字典
void dictRelease(dict *d) {

    _dictClear(d, &d->ht[0]);
    _dictClear(d, &d->ht[1]);

    free(c);
}

/**
 * 在字典中查找给定key所定义的节点
 * 
 * 如果key不存在，返回NULL
 */
dictEntry *dictFind(dict *d, const void *key) {

    dictEntry *he;
    unsigned int h, idx, table;

    if (d->ht[0].size == 0) return NULL;

    if (dictIsRehashing(d)) _dictRehashStep(d);

    // 计算哈希值
    h = dictHashKey(d, key);

    // 在两个哈希表中查找
    for (table = 0; table <= 1; table++) {
        // 索引值
        idx = h & d->ht[table].sizemark;
        // 节点链表
        he = d->ht[table].table[idx];
        /**
         * 在链表中查找
         * 因为链表的元素数量通常为1，或者维持在一个很小的比率
         * 因此可以将这个操作看作 O(1)
         */
        while (he) {
            // 找到并返回
            if (dictCompareKeys(d, key, he->key)) {
                return he;
            }
            he = he->next;
        }

        /**
         * 如果rehash并不在进行中
         * 那么无须查找ht[1]
         */
        if (!dictIsRehashing(d)) return NULL;
    }
    return NULL;
}