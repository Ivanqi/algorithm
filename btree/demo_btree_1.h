#ifndef __BTREE_H__
#define __BTREE_H__

#define M 4
#define KEY_NUM (2 * M - 1)

typedef int type_t;

typedef struct btree_t {
    int num;
    char leaf;              // 是否是叶子节点
    type_t key[KEY_NUM];
    struct btree_t* child[KEY_NUM + 1];
} btree_t, btnode_t;

btree_t* btree_create();
btree_t* btree_insert(btree_t *btree, type_t key);
btree_t* btree_delete(btree_t *btree, type_t key);
btree_t* btree_search(btree_t *btree, int *index);

static btree_t* btree_insert_nonfull(btree_t *btree, type_t key);
static btree_t* btree_split_child(btree_t *parent, int pos, btree_t *child);
static int btree_find_index(btree_t *btree, type_t key, int *ret);

#endif