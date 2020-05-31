#ifndef BTREE_H
#define BTREE_H

#define BTree_D 2
#define ORDER (BTree_D * 2)

typedef int KeyType;
typedef struct BTNode {
    int keynum;                     // 结点中关键字的个数， keynum < ORDER
    KeyType key[ORDER - 1];         // 关键字向量为key[0... keynum - 1]
    struct BTNode* child[ORDER];    // 孩子指针向量为child[0....keynum]
    char isleaf;                    // 是否是叶子结点标志
}BTNode;

typedef BTNode* BTree;  // 定义BTree

// 给定数据集data，创建BTree
void BTree_create(BTree* tree, const KeyType* data, int length);

// 销毁BTree , 释放内存空间
void BTree_destory(BTree* tree);

// 在BTree 中插入关键字Key
void BTree_insert(BTree* tree, KeyType key);

// 在 BTree 中 移除关键字
void BTree_remove(BTree* tree, KeyType key);

// 深度遍历BTree打印各层结点
void BTree_print(const BTree tree, int layer);

// BTree 中查找关键字 key
// 成功时返回找到的节点的地址及 key 在其中的位置 *pos 
// 失败时返回 NULL 及查找失败时扫描到的节点位置 *pos
BTree* BTree_search(const BTree tree, int key, int* pos); 

#endif /* BTREE_H */