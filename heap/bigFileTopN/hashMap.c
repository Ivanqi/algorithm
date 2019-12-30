#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hashMap.h>

HashMap *hash_map_create(hash_code_func hash_func) {

    HashMap *h = (HashMap*)malloc(sizeof(HashMap));

    assert(h != NULL);

    h->hTabs = (HashMapNode**)malloc(sizeof(HashMapNode*) * hash_map_init_size );
    assert(h->hTabs != NULL);

    h->header = NULL;

    h->count = 0;
    h->size = hash_map_init_size;
    h->hash_func = hash_func;

    int i;
    for (i = 0; i < hash_map_init_size; i++) {
        h->hTabs[i] = NULL;
    }
    h->header = init_list_head();
    assert(h->header != NULL);
    return h;
}

int hash_map_insert(HashMap *h, void *key) {

    if (h == NULL) return 1;


    int hPos = h->hash_func(h, key);
    HashMapNode *cur = h->hTabs[hPos];
    
    if (cur == NULL) {
        cur = (HashMapNode*)malloc(sizeof(HashMapNode));
        if (cur == NULL) return 1;

        cur->key = key;
        cur->num = 1;
        h->hTabs[hPos] = cur;
        list_add_tail(cur, h->header);
    } else {
        ++cur->num;
    }
    return 0;
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