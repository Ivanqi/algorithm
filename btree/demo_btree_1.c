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

int btree_find_index(btree_t *btree, type_t key, int *ret) {
    int i, num;

    /**
     * 循环中止条件
     *  ret == 0 意味着找到了key
     *  ret > 0 && i < num 意味着没找到key
     *  ret < 0 && i == num 意味着没找到key且超出数组范围
     */
    for (i = 0, num = btree->num; i < num && (*ret = btree->key[i] - key) < 0; ++i);
    return i;
}

btree_t* btree_delete(btree_t *btree, type_t key) {

    int index, ret, i;
    btree_t *preceding, *successor;
    btree_t *child, *sibling;
    type_t replace;

    index = btree_find_index(btree, key, &ret);

    if (btree->leaf && !ret) {
        /**
         * 情况1： 如果是叶子节点就直接删除
         */
        memmove(&btree->key[index], &btree->key[index + 1], sizeof(type_t) * (btree->num - index - 1));
        --btree->num;
        return btree;
    } else if (btree->leaf && ret) {
        // 没有找到节点
        return btree;
    }

    if (!ret) {
        /**
         * 情况2: 如果key在节点x中且节点x是中间节点，那么执行以下动作
         */
        preceding = btree->child[index];        // 左子树
        successor = btree->child[index + 1];    // 右子树

        if (preceding->num >= M) {  // 情况2-1
            /**
             * 情况2-1
             *  被删除节点的子树的个数大于M值，然后把该子树的最后一个元素上移到父节点的问题
             *  且递归删除，该子树的最后一个元素的值
             */
            replace = preceding->key[preceding->num - 1];
            btree->child[index] = btree_delete(preceding, replace);
            btree->key[index] = replace;
            return btree;
        }

        if (successor->num >= M) {  // 情况2-2
            /**
             * 情况2-2
             *  被删除节点的子树的个数大于M值，然后把该子树的第一个元素上移到父节点
             *  且递归删除，该子树的第一个元素的值
             */
            replace = successor->key[0];
            btree->child[index + 1] = btree_delete(successor, replace);
            btree->key[index] = replace;
            return btree;
        }

        if ((preceding->num == M - 1) && (successor->num == M - 1)) {   // 情况2-3：两个节点刚脱贫
            /**
             * 情况2-3
             *  左右孩子节都刚脱贫，删除前需要孩子节点的合并操作
             *  合并是将key下移至左孩子节点，并将右孩子节点合并到左孩子节点中
             *  删除右孩子节点，在父节点node中移除key和指向右孩子节点的指针
             *  然后在合并了的左孩子节点中递归删除元素
             */
            preceding->key[preceding->num++] = key;
            // 右子节点合并到左子节点
            memmove(&preceding->key[preceding->num], &successor->key[0], sizeof(type_t) * (successor->num));
            memmove(&preceding->child[preceding->num], &successor->child[0], sizeof(btree_t*) * (successor->num + 1));
            preceding->num += successor->num;

            
            if (btree->num - 1 > 0) {
                // 覆盖右子节点
                memmove(&btree->key[index], &btree->key[index + 1], sizeof(type_t) * (btree->num - index - 1));
                memmove(&btree->child[index + 1], &btree->child[index + 2], sizeof(btree_t *) * (btree->num - index - 1));
                --btree->num;
            } else {
                free(btree);
                btree = preceding;
            }

            free(successor);

            // 在左子节点中删除key
            btree_delete(preceding, key);

            return btree;
        }
    }

    // 节点中未找到key的情况
    if ((child = btree->child[index]) && child->num == M - 1) {     // 情况3: key不在中间节点中，而是在中间节点的子节点中

        /**
         * 情况3-1
         *  选择富有的相邻兄弟节点
         *  如果所在孩子节点相邻的兄弟节点中有节点至少包含M个关键字
         *  将node的一个关键字key[index]下移到child中，将相对富有的相邻兄弟节点中的一个关键字上移到node中
         *  然后在child孩子节点递归删除key
         */
        if ((index < btree->num) && (sibling = btree->child[index + 1]) && (sibling->num >= M)) {
            // 相邻右兄弟相对富有，则该孩子先向父节点借一个元素，右兄弟中第一个元素上移至父节点所借位置，并进行相应调整
            child->key[child->num++] = btree->key[index];
            btree->key[index] = sibling->key[0];

            child->child[child->num] = sibling->child[0];

            sibling->num--;
            memmove(&sibling->key[0], &sibling->key[1], sizeof(type_t*) * (sibling->num));
            memmove(&sibling->child[0], &sibling->child[1], sizeof(btree_t*) * (sibling->num + 1));

        } else if ((index > 0) && (sibling = btree->child[index - 1]) && (sibling->num >= M)) {
            // 相邻左兄弟相对富有，则该孩子向父节点借一个元素，左兄弟中的最后元素上移至父节点所借位置，并进行相应调整
            memmove(&child->key[1], &child->key[0], sizeof(type_t) * child->num);
            memmove(&child->child[1], &child->child[0], sizeof(btree_t*) * (child->num + 1));
            child->key[0] = btree->key[index - 1];
            btree->key[index - 1] = sibling->key[sibling->num - 1];
            child->child[0] = sibling->child[sibling->num];

            child->num++;
            sibling->num--;

        }

        /**
         * 情况3-2
         *  相邻兄弟节点刚脱贫。删除前需要兄弟结合的合并操作
         *  如果所在孩子节点的相邻兄弟节点都只包含 M - 1个关键字
         *  将child与其一相邻节点合并，并将node中的一个关键字下降到合并节点中
         *  再在node中删除那个关键字和相关指针，若node的key为空，删除，并调整根为合并节点
         *  最后，在相关孩子节点child中递归删除key
         */
        if ((index < btree->num) && (sibling = btree->child[index + 1]) && (sibling->num == M - 1)) {
            // 相邻右节点刚好脱贫，则该左节点向父节点借一个元素。然后把右节点合并到左节点
            child->key[child->num++] = btree->key[index];
            memmove(&child->key[child->num], &sibling->key[0], sizeof(type_t) * sibling->num);
            memmove(&child->child[child->num], &sibling->child[0], sizeof(btree_t*) * (sibling->num + 1));
            child->num += sibling->num;

            if (btree->num - 1 > 0) {
                // 父节点借了一个节点给左子节点，要把空洞节点删除。且把右子节点原来的位置删除
                memmove(&btree->key[index], &btree->key[index + 1], sizeof(type_t) * (btree->num - index - 1));
                memmove(&btree->child[index + 1], &btree->child[index + 2], sizeof(btree_t*) * (btree->num - index - 1));
                btree->num--;
            } else {
                free(btree);
                btree = child;
            }

            free(sibling);

        } else if ((index > 0) && (sibling = btree->child[index - 1]) && (sibling->num = M - 1)) {
            // 相邻左节点刚好脱贫，则该左节点向父节点借一个元素。然后把右节点合并到左节点中
            sibling->key[sibling->num] = btree->key[index - 1];
            memmove(&sibling->key[sibling->num], &child->key[0], sizeof(type_t) * child->num);
            memmove(&sibling->child[sibling->num], &child->child[0], sizeof(btree_t*) * (child->num + 1));
            sibling->num += child->num;

            if (btree->num - 1 > 0) {
                // 父节点借了一个节点给左子节点，要把空洞节点删除。且把右子节点原来的位置删除
                memmove(&btree->key[index - 1], &btree->key[index], sizeof(type_t) * (btree->num - index));
                memmove(&btree->child[index], &btree->child[index + 1], sizeof(btree_t*) * (btree->num - index));
                btree->num--;
            } else {
                free(btree);
                btree = sibling;
            }

            free(child);

            child = sibling;
        }
    }

    btree_delete(child, key);
    return btree;
}

btree_t* btree_search(btree_t *btree, type_t key, int *index) {

    int i;
    *index = -1;

    // 寻找下标
    for (i = 0; i < btree->num && key > btree->key[i]; ++i)
        ;
    
    // 确认下标i对应的值是否与key相等
    if (i < btree->num && key == btree->key[i]) {
        *index = i;
        return btree;
    }

    // 如果是叶子节点还是没有找到证明，树中没有该值
    if (btree->leaf) {
        return NULL;
    } else {
        return btree_search(btree->child[i], key, index);
    }

}

#define NUM 20

int main() {

    btree_t *btree;
    btnode_t *node;
    int index, i;
    
    if (!(btree = btree_create())) {
        exit(-1);
    }

    // insert
    for (i = 1; i < NUM; i++) {
        printf("%d\n", i);
        btree = btree_insert(btree, i);
    }

    // search
    for (i = 1; i < NUM; ++i) {
        node = btree_search(btree, i, &index);

        if (!node || index == -1) {
            printf("insert error!\n");
            return -1;
        }
    }
    
    // delete
    for (i = 1; i < NUM; ++i) {
        btree = btree_delete(btree, i);
        btree = btree_insert(btree, i);
    }

    return 0;
}