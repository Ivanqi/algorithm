#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stirng.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>

#include "demo_dict_2.h"

static int dict_can_resize = 1;
static unsigned int dict_force_resize_ratio = 5;

static int _dictExpandIfNeeded(dict *ht);
static unsigned long _dictNextPower(unsigned long size);
static long _dictKeyIndex(dict *ht, const void *key, uint64_t hash, dictEntry **existing);
static int _dictInit(dict *ht, dictType *type, void *privDataPtr);

static uint8_t dict_hash_function_seed[16];

void dictSetHashFunctionSeed(uint8_t *seed) {

    memcpy(dict_hash_function_seed, seed, sizeof(dict_hash_function_seed));
}

uint8_t *dictGetHashFunctionSeed(void) {

    return dict_hash_function_seed;
}

uint64_t siphash(const uint8_t *in, const size_t inlen, const uint8_t *k);
uint64_t siphash_nocase(const uint8_t *in, const size_t inlen, const uint8_t *k);

uint64_t dictGenHashFunction(const void *key, int len) {

    return siphash(key, len, dict_hash_function_seed);
}

uint64_t dictGenCaseHashFunction(const unsigned char *buf, int len) {

    return siphash_nocase(buf, len, dict_hash_function_seed);
}

// 重置哈希表的各项属性
static void _dictReset(dictht *ht) {

    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
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
    d->rehashidx = -1;
    d->iterators = 0;

    return DICT_OK;
}

// 对字典进行紧缩，让节点数/桶数的比率接近 <= 1
int dictResize(dict *d) {

    int minimal;
    
    // 不能在 dict_can_resize 为假
    // 或者字典正在 rehash时调用
    if (!dict_can_resize || dictIsRehashing(d)) return DICT_ERR;

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

    if (dictIsRehashing(d) || d->ht[0].used > size) {
        return DICT_ERR;
    }

    dictht n;   // 新的哈希表

    // 计算哈希表的真实大小
    unsigned long realsize = _dictNextPower(size);

    if (realsize == d->ht[0].size) return DICT_ERR;

    // 创建并初始化新哈希表
    n.size = realsize;
    n.sizemask = realsize - 1;
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
    d->rehashidx = 0;
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

    int empty_vists = n * 10;   // 访问的最大空桶数
    if (!dictIsRehashing(d)) return 0;

    while (n-- && d->ht[0].used != 0) {
        dictEntry *de, *nextde;

        assert(d->ht[0].size > (unsigned) d->rehashidx);

        // 移动到数组中首个不为NULL链表的索引上
        while (d->ht[0].table[d->rehashidx] == NULL) {
            d->rehashidx++;
            if (--empty_vists == 0) return 1;
        }

        // 指向链表头
        de = d->ht[0].table[d->rehashidx];

        /**
         * 将链表内的所有元素从ht[0]迁移到ht[1]
         * 因为桶内的元素通常只有一个，或者不多于某个特定比率
         * 所以可以将这个操作看作O(1)
         */
        while (de) {
            uint64_t h;

            nextde = de->next;

            // 计算元素在ht[1]的哈希值
            h = dictHashKey(d, de->key) & d->ht[1].sizemask;

            // 添加节点到 ht[1]，调整指针
            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;

            // 更新计数器
            d->ht[0].used--;
            d->ht[1].used++;

            de = nextde;
        }

        // 设置指针为NULL， 方便下次rehash时跳过
        d->ht[0].table[d->rehashidx] = NULL;

        // 前进至下一索引
        d->rehashidx++;
    }

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
        d->rehashidx = -1;

        // 通知调用者， rehash完毕
        return 0;
    }

    // 通知调用者，还有元素等待 rehash
    return 1;
}

