#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hashMap.h>

HashMapNode* init_list_head() {
    HashMapNode *node = (HashMapNode*)malloc(sizeof(HashMapNode));
    node->next = node;
    node->prev = node;
    return node;
}

HashMap *hash_map_create(int size, hash_code_func hash_func, keycmp_func hash_keycmp) {

    HashMap *h = (HashMap*)malloc(sizeof(HashMap));

    assert(h != NULL);

    h->hTabs = (HashMapNode**)malloc(sizeof(HashMapNode*) * size );
    assert(h->hTabs != NULL);

    h->header = NULL;

    h->count = 0;
    h->size = size;
    h->hash_func = hash_func;
    h->keycmp = hash_keycmp;

    int i;
    for (i = 0; i < size; i++) {
        h->hTabs[i] = NULL;
    }
    h->header = init_list_head();
    assert(h->header != NULL);
    return h;
}

int hash_map_insert(HashMap *h, void *key) {

    if (h == NULL) return 1;
    HashMapNode *cur = NULL;
    HashMapNode *prev = NULL;

    resize_hash_table_if_needed(h);

    int hPos = h->hash_func(h, key);
    cur = h->hTabs[hPos];
    while ((cur != NULL) && (h->keycmp(h, key, cur->key) != 0)) {
        prev = cur;
        cur = cur->hnext;
    }
    if (cur == NULL) {
        cur = (HashMapNode*)malloc(sizeof(HashMapNode));
        if (cur == NULL) return 1;
        cur->key = key;
        cur->num = 1;

        if (prev == NULL) {
            cur->hnext = h->hTabs[hPos];
            h->hTabs[hPos] = cur;
        } else {
            cur->hnext = prev->hnext;
            prev->hnext = cur;
        }
        
        h->count++;
        list_add_tail(cur, h->header);
    } else {
        ++cur->num;
    }
    return 0;
}

void resize_hash_table_if_needed(HashMap *h) {
    if (h->size - h->count < 1) {
        resize_hash(h);
    }
}

int resize_hash(HashMap *h) {
    int org_size = h->size;
    h->size = h->size * 2;
    h->count = 0;

    HashMapNode **new_node = (HashMapNode **)calloc(h->size, sizeof(HashMapNode **));
    HashMapNode *old_header = h->header;
    HashMapNode *new_header = init_list_head();
    h->hTabs = new_node;
    h->header = new_header;

    if (h->hTabs == NULL) return 1;

    HashMapNode *pos;
    int i;
    list_for_each(pos, old_header) {
        for (i = 0; i < pos->num;i++) {
            hash_map_insert(h, pos->key);
        }
        free(pos);
    }
    free(old_header);
}

void hash_map_destory(HashMap *h) {
    assert(h != NULL);
    HashMapNode *pos;
    list_for_each(pos, h->header) {
        free(pos);
    }
    free(h->hTabs);
    free(h);
}