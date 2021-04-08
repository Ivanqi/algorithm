#ifndef _LRU_LINKED_LIST_H
#define _LRU_LINKED_LIST_H

#define bool int
#define true 1
#define false -1

// ========================= 队列 ======================
typedef struct LinkQueue {
    char *key;
    struct LinkQueue *prev;
    struct LinkQueue *next;
} LinkQueue;

typedef struct List {
    int count;
    struct LinkQueue *head;
    struct LinkQueue *tail;
} List;

List* create_queue();
bool enqueue(List *list, char *data);

// ========================= hast table ======================

#define HASH_TABLE_INIT_SIZE 10
typedef struct _Bucket {
    char *key;
    void *value;
    struct _Bucket *next;
} Bucket;

typedef struct _HashTable {
    int size;       // 哈希表大小
    int elem_num;   // 已保存元素个数
    Bucket **buckets;
} HashTable;

static int hash_str(char *key) {
    int hash = 0;
    char *cur = key;
    while (*cur != '\0') {
        hash += *cur;
        ++cur;
    }
    return hash;
}

// 一次性扩容
static int hash_resize(HashTable *ht) {
    int org_size = ht->size;
    ht->size = ht->size * 2;
    ht->elem_num = 0;

    Bucket **bucket = (Bucket**)calloc(ht->size, sizeof(Bucket*));
    Bucket **org_buckets = ht->buckets;

    ht->buckets = bucket;

    int i = 0;
    for (i = 0; i < org_size; i++) {
        Bucket *cur = org_buckets[i];
        Bucket *tmp;

        while (cur) {
            hash_insert(ht, cur->key, cur->value);
            tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }

    free(org_buckets);
    return true;
}

static void resize_hash_table_if_needed(HashTable *ht) {
    if (ht->size - ht->elem_num < 1) {
        hash_resize(ht);
    }
}

static int HASH_INDEX(HashTable *ht, char *key) {
    int hash_num =  hash_str(key);
    return hash_num % ht->size;
}

int hash_init(HashTable *ht);   // 初始化哈希表
int hash_lookup(HashTable *ht, char *key, void **result);   // 根据key查找内容
int hash_insert(HashTable *ht, char *key, void *value);     // 将内容插入到哈希表中
int hash_remove(HashTable *ht, char *key);                  // 删除key所指向的内容
int hash_destory(HashTable *ht);

// ========================= LRU CACHE ======================

#define QLEN 10

typedef struct lru_list {
    int max_capacity;
    List *Q;
    HashTable *map;
} lru_list;

int lru_create(lru_list *lru);
int lru_enqueue(lru_list *lru, char *key, void *data);
int lru_dequeue(lru_list *lru);
int lru_get(lru_list *lru, char *key);
void lru_destory(lru_list *lru);

#endif