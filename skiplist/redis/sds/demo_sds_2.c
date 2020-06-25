#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include "demo_sds_2.h"

const char *SDS_NOINIT = "SDS_NOINIT";

// 获取不同header的长度
static inline int sdsHdrSize(char type) {
    
    switch (type & SDS_TYPE_MASK) {
        case SDS_TYPE_5:
            return sizeof(struct sdshdr5);
        case SDS_TYPE_8:
            return sizeof(struct sdshdr8);
        case SDS_TYPE_16:
            return sizeof(struct sdshdr16);
        case SDS_TYPE_32:
            return sizeof(struct sdshdr32);
        case SDS_TYPE_64:
            return sizeof(struct sdshdr64);
    }

    return 0;
}

/**
 * 长度在0和2^5-1之间，选用SDS_TYPE_5类型的header
 * 长度在2^5和2^8-1之间，选用SDS_TYPE_8类型的header
 * 长度在2^8和2^16-1之间，选用SDS_TYPE_16类型的header
 * 长度在2^16和2^32-1之间，选用SDS_TYPE_32类型的header
 * 长度大于2^32的，选用SDS_TYPE_64类型的header。能表示的最大长度为2^64-1
 */
static inline char sdsReqType(size_t string_size) {

    if (string_size < 1 << 5) {
        return SDS_TYPE_5;
    }

    if (string_size < 1 << 8) {
        return SDS_TYPE_8;
    }

    if (string_size < 1 << 16) {
        return SDS_TYPE_16;
    }

#if (LONG_MAX == LLONG_MAX)
    if (string_size < 1ll << 32) {
        return SDS_TYPE_32;
    }
    return SDS_TYPE_64;
#else
    return SDS_TYPE_32;
#endif
}

/**
 * 根据给定初始化值 init, 创建sds
 * 如果init 为 NULL, 那么创建一个buf只包含 \0 的sds结构体
 */
sds sdsnew(const char *init) {

    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}


// 创建一个不包含任何内容的的空SDS
sds sdsempty() {

    return sdsnewlen("", 0);
}

/**
 * 创建一个指定长度的sds
 * sdsnewlen创建一个长度为initlen的sds字符串，并使用init指向的字符数组（任意二进制数据）来初始化数据
 * 如果init为NULL，那么使用全0来初始化。它的实现中，我们需要注意的是
 *  1. 如果创建一个长度为0的空字符串，那么不使用SDS_TYPE_5类型的header,而是转而使用SDS_TYPE_8的类型的header
 *      1. 这是因为创建的空字符一般接下来的操作可能是追加数据，但SDS_TYPE_5类型的sds字符串不适合追加数据（会引发内存重新分配）
 *  2. 需要的内存空间一次性进行分配，其中包含三部分：header，数据，最后多余字节（harlen + initlen + 1）
 *  3. 初始化的sds字符串数据要在最后追加一个NULL结束符(s[initlen] = '\0')
 * 
 */
sds sdsnewlen(const void *init, size_t initlen) {

    void *sh;
    sds s;
    char type = sdsReqType(initlen);
    if (type == SDS_TYPE_5 && initlen == 0) {
        type = SDS_TYPE_8;
    }

    int hdrlen = sdsHdrSize(type);

    unsigned char *fp;  // flag 指针

    sh = malloc(hdrlen + initlen + 1);

    if (init == SDS_NOINIT) {
        init = NULL;
    } else if (!init) {
        memset(sh, 0, hdrlen + initlen + 1);
    }

    if (sh == NULL) return NULL;

    s = (char *)sh + hdrlen;
    fp = ((unsigned char *) s) - 1;

    switch (type) {
        case SDS_TYPE_5: {
            *fp = type | (initlen << SDS_TYPE_BITS);
            break;
        }

        case SDS_TYPE_8: {
            SDS_HDR_VAR(8, s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
            break;
        }

        case SDS_TYPE_16: {
            SDS_HDR_VAR(16, s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
        }

        case SDS_TYPE_32: {
            SDS_HDR_VAR(32,s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
            break;
        }

        case SDS_TYPE_64: {
            SDS_HDR_VAR(64,s);
            sh->len = initlen;
            sh->alloc = initlen;
            *fp = type;
            break;
        }
    }

    if (initlen && init) {
        memcpy(s, init, initlen);
    }

    s[initlen] = '\0';
    return s;
}
/**
 * 释放指定的SDS
 * 内存要整体释放，所以要先计算出header起始指针，把它传给free
 * */
void sdsfree(sds s) {

    if (s == NULL) return;
    free((char *)s - sdsHdrSize(s[-1]));
}

/**
 * 将一个 char 数组的前 len 个字节复制至 sds
 * 如果 sds 的 buf 不足以容纳要复制的内容，那么扩展 buf 的长度
 * 让 buf 的长度大于等于 len 。
 */
sds sdscpylen(sds s, const char *t, size_t len) {

    if (sdsalloc(s) < len) {
        s = sdsMakeRoomFor(s, len);
        if (s == NULL) return NULL;
    }

    memcpy(s, t, len);
    s[len] = '\0';
    sdssetlen(s, len);
    return s;
}

// 将给定的C字符复制到SDS里面，覆盖SDS原有的字符串
sds sdscpy(sds s, const char *t) {

    return sdscpylen(s, t, strlen(t));
}

// 对 sds 的 buf 进行扩展，扩展的长度不少于 addlen 。
sds sdsMakeRoomFor(sds s, size_t addlen) {

    void *sh, *newsh;
    size_t avail = sdsavail(s);
    char type, oldtype = s[-1] & SDS_TYPE_MASK;
    size_t len, newlen;
    int hdrlen;

    if (avail >= addlen) return s;

    len = sdslen(s);
    sh = (char *)s - sdsHdrSize(oldtype);
    newlen = (len + addlen);

    if (newlen < SDS_MAX_PREALLOC) {
        newlen *= 2;
    } else {
        newlen += SDS_MAX_PREALLOC;
    }

    type = sdsReqType(newlen);
    if (type == SDS_TYPE_5) type = SDS_TYPE_8;
    hdrlen = sdsHdrSize(type);

    if (oldtype == type) {
        newsh = realloc(sh, hdrlen + newlen + 1);
        if (newsh == NULL) return NULL;
        s = (char *) newsh + hdrlen;
    } else {
        newsh = malloc(hdrlen + newlen + 1);
        if (newsh == NULL) return NULL;
        memcpy((char *)newsh + hdrlen, s, len + 1);
        free(sh);

        s = (char *) newsh + hdrlen;
        s[-1] = type;
        sdssetlen(s, len);
    }
    
    sdssetalloc(s, newlen);
    return s;
}

sds sdscatprintf(sds s, const char *fmt, ...) {

    va_list ap;
    char *t;
    va_start(ap, fmt);
    t = sdscatvprintf(s, fmt, ap);
    va_end(ap);
    return t;
}

sds sdscatvprintf(sds s, const char *fmt, va_list ap) {

    va_list cpy;
    char staticbuf[1024], *buf = staticbuf, *t;
    size_t buflen = strlen(fmt) * 2;

    if (buflen > sizeof(staticbuf)) {
        buf = malloc(buflen);
        if (buf == NULL) return NULL;
    } else {
        buflen = sizeof(staticbuf);
    }

    while (1) {

        buf[buflen - 2] = '\0';
        va_copy(cpy, ap);
        vsnprintf(buf, buflen, fmt, cpy);
        va_end(cpy);

        if (buf[buflen - 2] != '\0') {
            if (buf != staticbuf) free(buf);
            buflen *= 2;

            buf = malloc(buflen);
            if (buf == NULL) return NULL;
            continue;
        }
        break;
    }

    t = sdscat(s, buf);
    if (buf != staticbuf) free(buf);
    return t;
}

// 按长度 len 扩展 sds ，并将 t 拼接到 sds 的末尾
sds sdscatlen(sds s, const void *t, size_t len) {
    
    size_t curlen = sdslen(s);

    s = sdsMakeRoomFor(s, len);
    if (s == NULL) {
        return NULL;
    }

    // 复制
    memcpy(s + curlen, t, len);

    // 更新 len 和 free属性
    sdssetlen(s, curlen + len);

    // 终结符
    s[curlen + len] = '\0';
    return s;
}

// 将给定C字符拼接到SDS字符串的末尾
sds sdscat(sds s, const char *t) {
    return sdscatlen(s, t, strlen(t));
}

int main () {

    {
        sds x = sdsnew("foo"), y;

        test_cond("创建字符串并获取长度", sdslen(x) == 3 && memcmp(x,"foo\0",4) == 0);
        sdsfree(x);

        x = sdsnewlen("foo", 2);
        test_cond("创建指定长度的字符串", sdslen(x) == 2 && memcmp(x,"fo\0",2) == 0);

        x = sdscat(x, "bar");
        test_cond("字符串连接", sdslen(x) == 5 && memcmp(x, "fobar\0", 6) == 0);

        x = sdscpy(x, "a");
        test_cond("sdscpy() 复制一个短字符串", sdslen(x) == 1 && memcmp(x, "a\0", 2) == 0);

        x = sdscpy(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk");
        test_cond("sdscpy() 复制一个长字符串", sdslen(x) == 33 && memcmp(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk\0", 34) == 0);
        sdsfree(x);

        x = sdscatprintf(sdsempty(), "%d", 123);
        test_cond("sdscatprintf() 基础例子", strlen(x) == 3 && memcmp(x, "123\0", 4) == 0);
        sdsfree(x);

    }

    return 0;
}