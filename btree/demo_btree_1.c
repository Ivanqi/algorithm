#include "demo_btree_1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

btree_t* btree_create() {

    btree_t* btree;
    if (!(btree = (btree_t*)malloc(sizeof(btree_t)))) {
        printf("[%d]malloc error!\n", __LINE__);
        return NULL;
    }

    btree->num = 0;
    btree->leaf = 1;

    return btree;
}

btree_t* btree_insert(btree_t *btree, type_t key) {

    // 节点满了
    if (btree->num == KEY_NUM) {
        btree_t *p;
        if (!(p = (btree_t *)malloc(sizeof(btree_t)))) {
            printf("[%d]malloc error!\n", __LINE__);
            return NULL;
        }
        p->num = 0;
        p->child[0] = btree;
        p->leaf = 0;
        btree = btree_split_child(p, 0, btree);
    }
    // 节点没满
    return btree_insert_nonfull(btree, key);
}

btree_t* btree_split_child(btree_t *parent, int pos, btree_t *child) {

    btree_t *z;
    int i;

    if (!(z = (btree_t*)malloc(sizeof(btree_t)))) {
        printf("[%d]malloc error!\n", __LINE__);
        return NULL;
    }

    z->leaf = child->leaf;
    z->num = M - 1;

    // 把child key中下标大于M的值赋予给z key
    for (i = 0; i < M - 1; ++i) {
        z->key[i] = child->key[i + M];
    }

    if (!child->leaf) {
        // 把child child中下标大于M的值赋予给 z child
        for (i = 0; i < M; ++i) {
            z->child[i] = child->child[i + M];
        }
    }
    
    child->num = M - 1;

    // 移位，比pos大的值，都要往后移动
    for (i = parent->num; i > pos; --i) {
        parent->child[i + 1] = parent->child[i];
    }
    parent->child[pos + 1] = z;

    // 移位，比pos大的值，都要往后移动
    for (i = parent->num - 1; i >= pos; --i) {
        parent->key[i + 1] = parent->key[i];
    }
    // 取child的中值给pos
    parent->key[pos] = child->key[M - 1];

    parent->num++;

    return parent;
}

btree_t* btree_insert_nonfull(btree_t *btree, type_t key) {
    int i;
    i = btree->num - 1;

    // 如果是叶子节点，就直接查找值
    if (btree->leaf) {
        // 找出btree->key中小于key的位置
        while (i >= 0 && key < btree->key[i]) {
            btree->key[i + 1] = btree->key[i];
            --i;
        }

        // 没找到,直接在后面增加
        btree->key[i + 1] = key;
        btree->num++;
    } else {
        // 如果不是叶子节点，通过查找btree->key的范围，找到它的子节点，然后继续查找
        while (i >= 0 && key < btree->key[i]) {
            --i;
        }
        ++i;
        // 子节点满了
        if (btree->child[i]->num == KEY_NUM) {
            btree_split_child(btree, i, btree->child[i]);
            if (key > btree->key[i]) {
                ++i;
            }
        }
        btree_insert_nonfull(btree->child[i], key);
    }
    return btree;
}

#define NUM 20000

int main() {

    btree_t *btree;
    int i;
    
    if (!(btree = btree_create())) {
        exit(-1);
    }

    for (i = 1; i < NUM; i++) {
        btree = btree_insert(btree, i);
    }

    return 0;
}