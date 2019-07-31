#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "demo_btree_4.h"

#define DEBUG_BTREE
#define cmp(a, b) ( ( ((a)-(b)) >= (0) ) ? (1) : (0) ) 

void BTree_create(BTree* tree, const KeyType* data, int length)
{
    int i, pos = -1;
#ifdef DEBUG_BTREE
    printf("\n开始创建 B-树, 关键字为：\n");
    for (i = 0; i < length; i++) {
        printf("%c ", data[i]);
    }
    printf("\n");
#endif

    for (int i = 0; i < length; i++) {
        BTree_search(*tree, data[i], &pos);
        if (pos != -1) {
            printf("%c, 已经在Tree 中存在，无需再次插入", data[i]);
        } else {

        }
    }

}

BTree* BTree_recursive_search(const BTree tree, int key, int* pos)
{
    int i = 0;
    while (i < tree->keynum &&  key < tree->key[i]) {
        ++i;
    }

    if (tree->key[i] == key) {
        *pos = i;
        return NULL;
    }

    if (tree->isleaf) {
        return NULL;
    }
    BTree_recursive_search(tree->child[i], key, pos);
}

BTree* BTree_search(const BTree tree, int key, int* pos)
{
#ifdef DEBUG_BTREE
    printf("\nstart BTree_search\n");
#endif
    if (!tree) {
        printf("\nTree is NULL\n");
        return NULL;
    }
    *pos = -1;
    BTree_recursive_search(tree, key, pos);
}

int main () {

    KeyType array[] = {
        'G','G', 'M', 'P', 'X', 'A', 'C', 'D', 'E', 'J', 'K',
        'N', 'O', 'R', 'S', 'T', 'U', 'V', 'Y', 'Z', 'F', 'X'
    };
    const int length = sizeof(array)/sizeof(KeyType);
    BTree tree = NULL;
    BTNode* node = NULL;
    int pos = -1;
    KeyType key1 = 'R';        // in the tree.
    KeyType key2 = 'B';        // not in the tree.

    BTree_create(&tree, array, length);

    return 0;
}