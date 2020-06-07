#ifndef BPlusTree_h
#define BPlusTree_h

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1) / 2 : M / 2)

typedef struct BPlusNode *BPlusTree, *Position;
typedef int KeyType;

struct BPlusNode {
    int KeyNum;
    KeyType Key[M + 1];
    BPlusTree Children[M + 1];
    BPlusTree Next;
};

// 初始化
extern BPlusTree Initialize();

// 插入
extern BPlusTree Insert(BPlusTree T, KeyType Key);

// 删除
extern BPlusTree Remove(BPlusTree T, KeyType Key);

// 销毁
extern BPlusTree Destory(BPlusTree T);

// 遍历节点
extern void Traval(BPlusNode T);

// 遍历树叶节点的数据
extern void TravelData(BPlusTree T);

#endif