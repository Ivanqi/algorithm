#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "demo_sds_1.h"


/**
 * 根据给定初始化值 init, 创建sds
 * 如果init 为 NULL, 那么创建一个buf只包含 \0 的sds结构体
 */
sds sdsnew(const char *init) {
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}

/**
 * 创建一个指定长度的sds
 * 如果给定了初始化值 init 的话，
 */
sds sdsnewlen(const void *init, size_t initlen){
    struct sdshdr *sh;

    if (init) {
        sh = malloc(sizeof(struct sdshdr) + initlen + 1);
    } else {
        sh = calloc(initlen + 1, sizeof(struct sdshdr));
    }

    // 内存不足，分配失败
    if (sh == NULL) return NULL;

    sh->len = initlen;
    sh->free = 0;

    /**
     * 如果给定了 init 且 initlen不为0的话
     * 那么将init的内容复制至 sds buf
     */
    if (initlen && init) {
        memcpy(sh->buf, init, initlen);
    }

    // 加上终结符
    sh->buf[initlen] = '\0';

    // 返回buf ，而不是整个sdshdr
    return (char*)sh->buf;
}

// 创建一个不包含任何内容的的空SDS
sds sdsempty() {
    return sdsnewlen("", 0);
}

// 创建一个给定SDS的副本(copy)
sds sdsdup(const sds s) {
    return sdsnewlen(s, strlen(s));
}

// 释放指定的SDS
void sdsfree(sds s) {
    if (s == NULL) return;
    free(s - sizeof(struct sdshdr));
}

// 更新给定 sds 所对应的 sdshdr 结构的 free 和 len 属性
void sdsupdatelen(sds s) {
    struct sdshdr *sh = (void*)(s - (sizeof(struct sdshdr)));

    // 计算正确的buf长度
    int reallen = strlen(s);

    // 更新属性
    sh->free += (sh->len - reallen);
    sh->len = reallen;
}

// 清除SDS保存的字符串内容
void sdsclear(sds s) {
    struct sdshdr *sh = (void*)(s - (sizeof(struct sdshdr)));

    sh->free += sh->len;
    sh->len = 0;
    sh->buf[0] = '\0';
}

int main() {

    struct sdshdr *sh;
    sds x = sdsnew("foo"), y;

    test_cond("创建字符串并获取长度", sdslen(x) == 3 && memcmp(x,"foo\0",4) == 0);
    sdsfree(x);

    x = sdsnewlen("foo", 2);
    test_cond("创建指定长度的字符串", sdslen(x) == 2 && memcmp(x,"fo\0",2) == 0);
    sdsfree(x);

    
}