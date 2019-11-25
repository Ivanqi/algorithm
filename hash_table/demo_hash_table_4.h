#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_ 1

#define TEST(tcase) printf(">>> [START CASE] " tcase "<<<\n")
#define PASS(tcase) printf(">>> [PASSED] " tcase " <<<\n")

#define HASH_TABLE_INIT_SIZE 6
#define HASH_INDEX(ht, key)(hash_str(key) % (ht)->size)
#endif

#if defined(DEBUG)
    #define LOG_MSG printf
#else
    #define LOG_MSG(...)

#define SUCCESS 0
#define FAILED -1

typedef struct _Bucket {
    char *key;
    void *value;
    struct _Bucket *next;
} Bucket;

typedef struct _HashTable {
    int size;               // 哈希表大小
    int elem_num;           // 已保存元素的个数
    Bucket **buckets;
} HashTable;

int hash_init(HashTable *ht);   // 初始化哈希表
int hash_lookup(HashTable *ht, char *key, void **result);   // 根据key查找内容
int hash_insert(HashTable *ht, char *key, void *value);     // 将内容插入到哈希表中
int hash_remove(HashTable *ht, char *key);  // 删除key所指向的内容
int hash_destroy(HashTable *ht);

#endif