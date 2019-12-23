#ifndef _LRU_HASH_TABLE_
#define _LRU_HASH_TABLE_ 1

#define HASH_TABLE_INIT_SIZE 6
#define SUCCESS 0
#define FAILED -1

typedef struct _Bucket {
    char *key;
    void *value;
    struct _Bucket *next;
    struct _Bucket *prev;
    struct _Bucket *hnext;
} Bucket;

typedef struct _HashTable {
    int size;
    int elem_num;
    Bucket **buckets;
} HashTable;

int hash_init(HashTable *ht);    // 初始化哈希表
int hash_insert(HashTable *ht, char *key, void *value);  // 将内容插入到哈希表中
int hash_search(HashTable *ht, char *key, void **result);   // 根据key查找内容
int hash_delete(HashTable *ht, char *key);  // 根据key删除内容
int hash_desotry(HashTable *ht);   // 销毁hash

void resize_hash_table_if_needed(HashTable *ht);
int resize_hash(HashTable *ht);
int hash_str(char *key);
int hash_index(HashTable *ht, int hash);

#endif