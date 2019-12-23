#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "demo_hash_table_5.h"

Bucket *bt = NULL;

int hash_init(HashTable *ht) {

    ht->size = HASH_TABLE_INIT_SIZE;
    ht->elem_num = 0;
    ht->buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));
    if (ht->buckets == NULL) return FAILED;
    return SUCCESS;
}

Bucket* create_node() {
    Bucket *node = (Bucket *)malloc(sizeof(Bucket));
    assert(node != NULL);
    node->prev = node;
    node->next = node;
    node->hnext = node;
    return node;
}

int hash_insert(HashTable *ht, char *key, void *value) {
    resize_hash_table_if_needed(ht);

    int index = hash_index(ht, hash_str(key));

    Bucket *org_bucket = ht->buckets[index];
    Bucket *tmp_bucket = org_bucket;

    while (tmp_bucket) {
        if (strcmp(tmp_bucket->key, key)) {
            tmp_bucket->value = value;
            return SUCCESS;
        }
        tmp_bucket = tmp_bucket->hnext;
    }

    Bucket *new_bucket = (Bucket *)malloc(sizeof(Bucket));
    new_bucket->key = key;
    new_bucket->value = value;
    new_bucket->hnext = NULL;

    new_bucket->prev = bt;
    new_bucket->next = bt->next;

    bt->next->prev = new_bucket;
    bt->next = new_bucket;

    if (org_bucket != NULL) {
        new_bucket->hnext = org_bucket;
    }
    ht->buckets[index] = new_bucket;
    ht->elem_num++;
    return SUCCESS;    
}

int hash_str(char *key) {
    int hash = 0;
    char *cur = key;
    while (*cur != '\0') {
        hash += *cur;
        ++cur;
    }
    return hash;
}

int hash_index(HashTable *ht, int hash) {
    return hash % ht->size;
}

void resize_hash_table_if_needed(HashTable *ht) {
    if (ht->size - ht->elem_num < 1) {
        resize_hash(ht);
    }
}

int resize_hash(HashTable *ht) {
    int org_size = ht->size;
    ht->size = ht->size * 2;
    ht->elem_num = 0;

    Bucket **new_bucket = (Bucket **)calloc(ht->size, sizeof(Bucket **));
    Bucket **old_bucket = ht->buckets;
    ht->buckets = new_bucket;

    if (ht->buckets == NULL) return FAILED;

    int i;
    ;
    for (i = 0; i < org_size; i++) {
        Bucket *cur = old_bucket[i];
        Bucket *tmp;
        while (cur) {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            hash_insert(ht, cur->key, cur->value);
            tmp = cur;
            cur = cur->hnext;
            free(tmp);
        }
    }
}

int hash_desotry(HashTable *ht) {
    int i = 0;
    for (i = 0; i < ht->size; i++) {
        Bucket *cur = ht->buckets[i];
        Bucket *tmp;
        while (cur != NULL) {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            tmp = cur;
            cur = cur->hnext;
            free(tmp);
        }
    }
    free(ht->buckets);
    free(bt);
    return SUCCESS;
}

int hash_search(HashTable *ht, char *key, void **result) {

    int index = hash_index(ht, hash_str(key));
    Bucket *bucket = ht->buckets[index];

    if (bucket == NULL) return FAILED;

    while (bucket != NULL) {
        if (strcmp(bucket->key, key) == 0) {
            *result = bucket->value;
            return SUCCESS;
        }
        bucket = bucket->hnext;
    }
    return FAILED;
}

int hash_delete(HashTable *ht, char *key) {
    int index = hash_index(ht, hash_str(key));
    Bucket *bucket = ht->buckets[index];

    if (bucket == NULL) return FAILED;

    Bucket *cur = bucket;
    Bucket *tmp;
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            tmp = cur;
            free(tmp);
            return SUCCESS;
        }
        cur = cur->hnext;
    }
    return FAILED;
}


void hash_insert_test(HashTable *ht) {
    int  int1 = 12;
	int  int2 = 10;
	char str1[] = "Hello PITI";
	char str2[] = "Value";
    char str3[] = "Hello Ivan";

    hash_insert(ht, "KeyInt", &int1);
	hash_insert(ht, "asdfKeyStrass", str1);
	hash_insert(ht, "K13eyStras", str1);
	hash_insert(ht, "KeyStr5", str1);
	hash_insert(ht, "KeyStr1", str1);
    hash_insert(ht, "KeyStr1", str1);
    hash_insert(ht, "KeyStr2", str1);
    hash_insert(ht, "KeyStr3", str2);
    hash_insert(ht, "KeyStr6", str1);
}

void hash_search_test(HashTable *ht) {
    int *j = NULL;
	char *find_str = NULL;
    int ret;

    ret = hash_search(ht, "KeyInt", (void **)&j);
    if (ret == SUCCESS) {
        printf("KeyInt searh Success: %d\n", *j);
    } else {
        printf("KeyInt searh failed\n");
    }


	ret = hash_search(ht, "KeyStr1", (void **)&find_str);
    
    if (ret == SUCCESS) {
        printf("KeyStr searh Success: %s\n", (char*)find_str);
    } else {
        printf("KeyStr searh failed\n");
    }
	
}

void hash_delete_test(HashTable *ht) {

    int *j = NULL;
	char *find_str = NULL;
    char strMyKey[] = "MyKeyValue";
	find_str = NULL;
    int result;

	result = hash_insert(ht, "My-Key", &strMyKey);
    printf("insert result:%d\n", result);
    // assert(result == SUCCESS);

	result = hash_delete(ht, "My-Key");
    printf("result1: %d\n", result);
	assert(result == SUCCESS);

	result = hash_search(ht, "My-Key", (void **)&find_str);
    printf("result2: %d\n", result);
	assert(find_str == NULL);
	assert(result == FAILED);
}

int main () {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    int ret = hash_init(ht);
    bt = create_node();
    assert(ret == SUCCESS);

    hash_insert_test(ht);
    // printf("\n --- \n");
    hash_search_test(ht);
    // printf("\n --- \n");
    hash_delete_test(ht);

    ret = hash_desotry(ht);
    assert(ret == SUCCESS);
    return SUCCESS;
}