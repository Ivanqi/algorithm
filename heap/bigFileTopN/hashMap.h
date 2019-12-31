#ifndef _HASH_MAP_
#define _HASH_MAP_

// 数据存放节点
typedef struct hash_map_node {
    void *key;  // 键
    int num;    // 数量
    struct hash_map_node *next;
    struct hash_map_node *prev;
    struct hash_map_node *hnext;
} HashMapNode;

typedef struct hash_map {
    HashMapNode **hTabs;    // 哈希桶
    HashMapNode* header;       // 双向循环链表
    int count;
    int size;                // 支持最大节点
    int (*hash_func)(struct hash_map *h, const void *key);    // 哈希函数
    int (*keycmp)(struct hash_map *h, const void *key1, const void *key2);
} HashMap;

typedef int (*hash_code_func)(HashMap *h, const void *key);    // 哈希函数
typedef int (*keycmp_func)(struct hash_map *h, const void *key1, const void *key2);

HashMapNode* init_list_head();
HashMap *hash_map_create(int size, hash_code_func hash_func, keycmp_func hash_keycmp);
HashMapNode *hash_map_delete();
void resize_hash_table_if_needed(HashMap *h);
int resize_hash(HashMap *h);

static inline int hash_str(const char *str) {
    int hash = 0;
    int seed = 33;
    while (*str) {
        hash = hash * seed + *str++;
    }
    return hash & (0x7FFFFFFF);
}

static inline int hash_index(HashMap *h, int hash) {
    return hash % h->size;
}

static inline int hash_code(HashMap *h, const void *key) {
    return hash_index(h, hash_str(key));
}

static inline int hash_keycmp(HashMap *h, const void *key1, const void *key2) {
    return strcmp(key1, key2);
}

// 添加new节点：将new添加到head之后，把new称为head的后继节点
static inline void list_add_head(HashMapNode *new, HashMapNode *head) {
    new->prev = head;
    new->next = head->next;

    head->next->prev = new;
    head->next = new;
}

// 添加new节点，将new添加到head之前，即将new添加到双链表的末尾
static inline void list_add_tail(HashMapNode *new, HashMapNode *head) {
    new->next = head;
    new->prev = head->prev;

    head->prev->next = new;
    head->prev = new;
}

// 双向链表是否为空
static inline int list_empty(HashMapNode *head) {
    return head->next == head;
}

// 遍历双向链表
#define list_for_each(pos, head) \
    for (pos = (head->next); pos != head; pos = pos->next)

#endif