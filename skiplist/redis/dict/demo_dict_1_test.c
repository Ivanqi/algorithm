#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo_dict_1.h"

unsigned int hashFunc_1(const void *key) {
    return dictIntHashFuncion((unsigned int *)key);
}

unsigned int hashFunc_2(const void *key) {
    return dictGenHashFunction(key, sizeof(key));
}

unsigned int hashFunc_3(const void *key) {
    const unsigned char *data = (const unsigned char *)key;
    int len = strlen(data);
    return dictGenCaseHashFunction(data, len);
}

void test_case_1() {
    dictType *type = (dictType *)malloc(sizeof(dictType));

    char privDataPtr[] = "privDataPtr";
    dict *d;

    type->hashFunction = hashFunc_2;

    d = dictCreate(type, privDataPtr);

    int i;
    for (i = 1; i < 7; i++) {
        dictAdd(d, (void*)&i, (void*)&i);
    }

    dictIterator *dIter = dictGetIterator(d);
    dictEntry *node;

    while ((node = dictNext(dIter)) != NULL) {
        printf("node key:%d, val:%d\n", *(void **)node->key, *(void **)node->v.val);
    }

    int find = 2;

    dictEntry *entry = dictFind(d, &find);
    if (entry) {
        printf("entry key:%d, val:%d\n", *(void **)entry->key, *(void **)entry->v.val);
    } else {
        printf("没找到对应的值\n");
    }
    dictRelease(d);
}

int main() {

    test_case_1();
    return 0;
}