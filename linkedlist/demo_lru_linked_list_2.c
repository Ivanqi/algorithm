#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "demo_lru_linked_list_2.h"

List* create_queue() {
    List *l = (List *)malloc(sizeof(List));
    assert(l != NULL);
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
    return l;
}

bool enqueue(List *list, char *data) {
    assert(list != NULL);
    LinkQueue *p = (LinkQueue*)malloc(sizeof(LinkQueue));
    assert(p != NULL);
    p->key = data;
    p->next = NULL;
    p->prev = NULL;

    if (list->head == NULL) {
        list->head = p;
    } else {
        list->tail->next = p;
        p->prev = list->tail;
    }

    list->tail = p;
    list->count++;
    return true;
}

char* dequeue(List *list) {
    assert(list != NULL);
    LinkQueue *tmp = list->head;
    char *key = tmp->key;
    
    if (list->head->next != NULL) {
        list->head->next->prev = NULL;
        list->head = list->head->next;
    }
   
    tmp->prev = NULL;
    tmp->next = NULL;

    list->count--;

    if (list->head == NULL) {
        list->tail = NULL;
    }

    return key;
}

char* dequeueTail(List *list) {
    assert(list != NULL);
    LinkQueue *tmp = list->tail;
    char *key = tmp->key;

    if (list->tail->prev != NULL) {
        list->tail->prev->next = NULL;
        list->tail = list->tail->prev;
    }

    tmp->next = NULL;
    tmp->prev = NULL;
    list->count--;

    if (list->tail == NULL) {
        list->head = NULL;
    }

    return key;
}

void destory(List *list) {
    if (list == NULL || list->count == 0) return;

    while (!list->count == 0) {
        dequeue(list);
    }
    free(list);
}

int hash_init(HashTable *ht) {

    assert(ht != NULL);
    ht->elem_num = 0;
    ht->size = HASH_TABLE_INIT_SIZE;
    
    ht->buckets = (Bucket**)calloc(ht->size, sizeof(Bucket*));
    assert(ht->buckets != NULL);

    int i;
    for (i = 0; i < ht->size; i++) {
        ht->buckets[i] = NULL;
    }

    return true;
}

int hash_insert(HashTable *ht, char *key, void *value) {
    assert(ht != NULL);
    assert(key != NULL);
    assert(value != NULL);

    resize_hash_table_if_needed(ht);

    int index = HASH_INDEX(ht, key);
    Bucket *org_bucket = ht->buckets[index];
    Bucket *tmp_bucket = org_bucket;

    while (tmp_bucket) {
        if (strcmp(tmp_bucket->key, key) == 0) {
            tmp_bucket->value = value;
            return true;
        }
        tmp_bucket = tmp_bucket->next;
    }

    Bucket *bucket = (Bucket*)malloc(sizeof(Bucket));
    assert(bucket != NULL);
    bucket->key = key;
    bucket->value = value;
    bucket->next = NULL;

    if (org_bucket != NULL) {
        org_bucket->next = bucket;
    }
    ht->buckets[index] = bucket;
    ht->elem_num++;
    return true;
}

int hash_lookup(HashTable *ht, char *key, void **result) {
    assert(ht != NULL);
    assert(key != NULL);

    int index = HASH_INDEX(ht, key);
    Bucket *bucket = ht->buckets[index];
    if (bucket == NULL) return false;

    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            *result = bucket->value;
            return true;
        }
        bucket = bucket->next;
    }

    return false;
}

int hash_remove(HashTable *ht, char *key) {
    assert(ht != NULL);
    assert(key != NULL);

    int index = HASH_INDEX(ht, key);
    Bucket *bucket = ht->buckets[index];
    if (bucket == NULL) return false;

    Bucket *prev = NULL;

    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            if (prev == NULL) {
                ht->buckets[index] = bucket->next;
            } else {
                prev->next = bucket->next;
            }
            free(bucket);
            return true;
        }
        prev = bucket;
        bucket = bucket->next;
    }
    return false;
}

int hash_destory(HashTable *ht) {
    assert(ht != NULL);
    int i;
    Bucket *cur = NULL;
    Bucket *tmp = NULL;

    for (i = 0; i < ht->size; i++) {
        cur = ht->buckets[i];
        while (cur) {
            tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    
    free(ht);
    return true;
}

int lru_create(lru_list *lru) {
    assert(lru != NULL);

    lru->max_capacity = QLEN;

    List *l = create_queue(l);
    lru->Q = l;

    HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
    int result = hash_init(ht);
    assert(result == true);

    lru->map = ht;
    assert(lru->map != NULL);

    return true;
    
}

int lru_enqueue(lru_list *lru, char *key, void *data) {
    assert(lru != NULL);

    if (lru->Q->count >= lru->max_capacity) {
        char *tmp = dequeueTail(lru->Q);
        char *find_str = NULL;
        int ret = hash_lookup(lru->map, tmp, (void **)&find_str);
        if (ret == true) {
            ret = hash_remove(lru->map, tmp);
            if (ret == false) {
                printf("lru_enqueue dequeueTail fail\n");
                return false;
            }
        } else {
            printf("hash_lookup: %s failed\n", tmp);
            return false;
        }
    }

    enqueue(lru->Q, key);
    hash_insert(lru->map, key, data);

    return true;
}

int lru_dequeue(lru_list *lru) {

    char *key = dequeue(lru->Q);
    char *find_str = NULL;
    int ret = hash_lookup(lru->map, key, (void **)&find_str);
    
    if (ret == true) {
        printf("key:%s, val:%s\n", key, find_str);
        ret = hash_remove(lru->map, key);
        if (ret == false) {
            printf("lru_dequeue dequeue fail\n");
        }
    }
    return true;
}

int lru_get(lru_list *lru, char *key) {
    char *find_str = NULL;
    int ret = hash_lookup(lru->map, key, (void **)&find_str);
    if (ret == true) {
        printf("key:%s, val:%s\n", key, find_str);
    }
    return true;
}

void lru_destory(lru_list *lru) {
    destory(lru->Q);
    hash_destory(lru->map);
    free(lru);
}


void test_link_queue() {
    ;
    List *l = create_queue();
    int i;
    enqueue(l, "a1");
    enqueue(l, "a2");
    enqueue(l, "a3");
    enqueue(l, "a4");
    enqueue(l, "a5");
    enqueue(l, "a6");
    enqueue(l, "a7");
    enqueue(l, "a8");

    // printf("dequeue val: %s\n", dequeueTail(&l));
    for (i = 0 ; i < 8; i++) {
       printf("dequeue val: %s\n", dequeue(l));
    }
    
    destory(l);
}

void test_hash_table() {
    HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
	int result = hash_init(ht);

    assert(result == true);

    int  int1 = 10;
	int  int2 = 20;
	char str1[] = "Hello TIPI";
	char str2[] = "Value";

    int *j = NULL;
	char *find_str = NULL;

    hash_insert(ht, "KeyInt", &int1);
	hash_insert(ht, "asdfKeyStrass", str1);
	hash_insert(ht, "K13eyStras", str1);
	hash_insert(ht, "KeyStr5", str1);
	hash_insert(ht, "KeyStr", str1);

    hash_lookup(ht, "KeyInt", (void **)&j);
	hash_lookup(ht, "KeyStr", (void **)&find_str);

    assert(strcmp(find_str, str1) == 0);
}

void test_lru_cache() {
    lru_list *lru = (lru_list*)malloc(sizeof(lru_list));
    int ret = lru_create(lru);

    lru_enqueue(lru, "name", "ivan");
    lru_enqueue(lru, "age", "18");
    lru_enqueue(lru, "high", "178cm");
    lru_enqueue(lru, "weigth", "65kg");
    lru_enqueue(lru, "nickname", "abc");
    lru_enqueue(lru, "school", "qqq");
    lru_enqueue(lru, "home", "next street");
    
    lru_dequeue(lru);
    lru_dequeue(lru);

    char *get = "home";

    lru_get(lru, get);

    lru_destory(lru);
}

int main () {
    test_lru_cache();
    return 0;
}