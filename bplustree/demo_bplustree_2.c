#include "demo_bplustree_2.h"

static KeyType Unavailable = INT_MAX;

// 生成节点并初始化
static BPlusTree MallocNewNode() {

    BPlusTree NewNode;
    int i;
    NewNode = malloc(sizeof(struct BPlusNode));
    if (NewNode == NULL) {
        exit(EXIT_FAILURE);
    }

    i = 0;
    while (i < M + 1) {
        NewNode->Key[i] = Unavailable;
        NewNode->Children[i] = NULL;
        i++;
    }

    NewNode->Next = NULL;
    NewNode->KeyNum = 0;

    return NewNode;
}

// 初始化
extern BPlusTree Initialize() {

    BPlusTree T;
    if (M < (3)) {
        printf("M最小值等于3！");
        exit(EXIT_FAILURE);
    }

    // 根节点
    T = MallocNewNode();

    return T;
}

static Position FindMostLeft(Position P) {

    Position Tmp;
    Tmp = P;

    while (Tmp != NULL && Tmp->Children[0] != NULL) {
        Tmp = Tmp->Children[0];
    }
    return Tmp;
}

static Position FindMostRight(Position P) {

    Position Tmp;
    Tmp = P;

    while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
        Tmp = Tmp->Children[Tmp->KeyNum - 1];
    }
    return Tmp;
}

// 寻找一个兄弟节点，其存储的关键字未满，否则返回NULL。 寻找刚脱贫的兄弟节点
static Position FindSibling(Position Parent, int i) {

    Position Sibling;
    int Limit;

    Limit = M;

    Sibling = NULL;

    if (i == 0) {
        if (Parent->Children[1]->KeyNum < Limit) {
            Sibling = Parent->Children[1];
        } 
    } else if (Parent->Children[i - 1]->KeyNum < Limit)  {  // 左节点
        Sibling = Parent->Children[i - 1];
    } else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit) { // 右节点
        Sibling = Parent->Children[i + 1];
    }

    return Sibling;
}

// 查找兄弟节点，其关键字大于 M/2；没有返回NULL
static Position FindSiblingKeyNum_M_2(Position Parent, int i, int *j) {

    int Limit;
    Position Sibling;
    Sibling = NULL;

    Limit = LIMIT_M_2;

    if (i == 0) {
        if (Parent->Children[1]->KeyNum > Limit) {
            Sibling = Parent->Children[1];
            *j = 1;
        }
    } else {
        if (Parent->Children[i - 1]->KeyNum > Limit) {
            Sibling = Parent->Children[i - 1];
            *j = i - 1;
        } else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum > Limit) {
            Sibling = Parent->Children[i + 1];
            *j = i + 1;
        }
    }

    return Sibling;
}

/**
 * 当要对X插入Key的时候，i是X在Parent的位置，j是Key要插入的位置
 * 当要对Parent插入X节点的时候，i是要插入的位置，Key和j值没有用
 */
static Position InsertElement(int isKey, Position Parent, Position X, KeyType Key, int i, int j) {

    int k;
    if (isKey) {
        // 插入Key
        k = X->KeyNum - 1;
        // Key[k]往后移动，空出j的位置
        while (k >= j) {
            X->Key[k + 1] = X->Key[k];
            k--;
        }

        X->Key[j] = Key;

        if (Parent != NULL) {
            Parent->Key[i] = X->Key[0];
        }

        X->KeyNum++;
    } else {
        // 插入节点

        // 对树叶节点进行连接
        if (X->Children[0] == NULL) {
            if (i > 0) {
                Parent->Children[i - 1]->Next = X;
            }
            X->Next = Parent->Children[i];
        }

        k = Parent->KeyNum - 1;
        // 移动位置
        while (k >= i) {
            Parent->Children[k + 1] = Parent->Children[k];
            Parent->Key[k + 1] = Parent->Key[k];
            k--;
        }

        Parent->Key[i] = X->Key[0];
        Parent->Children[i] = X;

        Parent->KeyNum++;
    }

    return x;
}

static Position RemoveElement(int isKey, Position Parent, Position X, int i, int j) {

    int k, Limit;

    if (isKey) {
        Limit = X->KeyNum;
        // 删除Key
        k = j + 1;
        while (k < Limit) {
            X->Key[k - 1] = X->Key[k];
            k++;
        }

        X->Key[X->KeyNum - 1] = Unavailable;
        Parent->Key[i] = X->Key[0];

        X->KeyNum--;
    } else {
        // 删除节点
        // 修改树叶节点的链接
        if (X->Children[0] == NULL && i > 0) {
            Parent->Children[i - 1]->Next = Parent->Children[i + 1];
        }
        Limit = Parent->KeyNum;
        k = i + 1;

        while (k < Limit) {
            Parent->Children[k - 1] = Parent->Children[k];
            Parent->Key[k - 1] = Parent->Key[k];
            k++;
        }

        Parent->Children[Parent->KeyNum - 1] = NULL;
        Parent->Key[Parent->KeyNum - 1] = Unavailable;

        Parent->KeyNum--;
    }

    return X;
}

/**
 * Src 和 Dst 是两个相邻的节点，i是Src在Parent中的位置
 * 将Src的元素移动到Dst中，n是移动元素的个数
 */
static Position MoveElement(Position Src, Position Dst, Position Parent, int i, int n) {
    
    KeyType TmpKey;
    Position Child;
    int j, SrcInFront;

    SrcInFront = 0;

    if (Src->Key[0] < Dst->Key[0]) {
        SrcInFront = 1;
    }

    j = 0;

    // 节点Src在Dst前面
    if (SrcInFront) {
        if (Src->Children[0] != NULL) {
            while (j < n) {
                Child = Src->Children[Src->KeyNum - 1];
                RemoveElement(0, Src, Child, Src->KeyNum - 1, Unavailable);
                InsertElement(0, Dst, Child, Unavailable, 0, Unavailable);
                j++;
            }
        } else {
            while (j < n) {
                TmpKey = Src->Key[Src->KeyNum - 1];
                RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);
                InsertElement(1, Parent, Dst, TmpKey, i + 1, 0);
                j++;
            }
        }

        Parent->Key[i + 1] = Dst->Key[0];
        // 将树叶节点重新连接
        if (Src->KeyNum > 0) {
            FindMostRight(Src)->Next = FindMostLeft(Dst);
        }
    } else {
        if (Src->Children[0] != NULL) {
            while (j < n) {
                Child = Src->Children[0];
                RemoveElement(0, Src, Child, 0, Unavailable);
                InsertElement(0, Dst, Child, Unavailable, Dst->KeyNum, Unavailable);
                j++;
            }
        } else {
            while (j < n) {
                TmpKey = Src->Key[0];
                RemoveElement(1, );
            }
        }
    }
}

static BPlusTree SplitNode(Position Parent, Position X, int i) {

    int j, k, Limit;
    Position NewNode;

    NewNode = MallocNewNode();

    k = 0;
    j = X->KeyNum / 2;
    Limit = X->KeyNum;

    while (j < Limit) {
        // 把X中子节点下标大于j的搬运到新的节点中
        if (X->Children[0] != NULL) {
            NewNode->Children[k] = X->Children[j];
            X->Children[j] = NULL;
        }
        // 设置新节点的Key的值
        NewNode->Key[k] = X->Key[j];
        // 把旧节点的Key值设置为Unavailable
        X->Key[j] = Unavailable;
        NewNode->KeyNum++;
        X->KeyNum--;
        j++;
        k++;
    }

    if (Parent != NULL) {
        InsertElement(0, Parent, NewNode, Unavailable, i + 1, Unavailable);
    } else {
        // 如果X是根，那么创建新的根并返回
        Parent = MallocNewNode();
        InsertElement(0, Parent, X, Unavailable, 0, Unavailable);
        InsertElement(0, Parent, NewNode, Unavailable, 1, Unavailable);
        return Parent;
    }

    return X:
}

static BPlusTree RecursiveInsert(BPlusTree T, KeyType Key, int i, BPlusTree Parent) {

    int j, Limit;
    Position Sibling;

    // 查找分支
    j = 0;
    while (j < T->KeyNum && Key >= T->Key[j]) {
        // 重复值不插入
        if (Key == T->Key[j]) {
            return T;
        }
        j++;
    }

    if (j != 0 && T->Children[0] != NULL) j--;

    if (T->Children[0] == NULL) {   // 树叶节点
        T = InsertElement(1, Parent, T, Key, i, j);
    } else {    // 内部节点
        T = Children[j] = RecursiveInsert(T->Children[j], Key, j, T);
    }

    // 调整节点
    Limit = M;

    if (T->KeyNum > Limit) {
        // 根
        if (Parent == NULL) {
            // 分裂节点
            T = SplitNode(Parent, T, i);
        } else {
            Sibling = FindSibling(Parent, i)
            if (Sibling != NULL) {
                // 将T的一个元素(Key或者Child)移动到Sibing中
                MoveElement(T, Sibling, Parent, i, 1);
            } else {
                // 分裂节点
                T = SplitNode(Parent, T, i);
            }
        }
    }
}

// 插入
extern BPlusTree Insert(BPlusTree T, KeyType Key) {
    return RecursiveInsert(T, Key, 0, NULL);
}