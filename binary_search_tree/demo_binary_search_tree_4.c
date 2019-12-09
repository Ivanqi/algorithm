#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* left;
    struct node* right;
} Node;

typedef struct {
    Node* root;
} Tree;

void insert(Tree* tree, int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    if (tree->root == NULL) {
        tree->root = node;
    } else {
        Node* tmp = tree->root;
        while (tmp != NULL) {
            if (data < tmp->data) {
                if (tmp->left == NULL) {
                    tmp->left = node;
                    return;
                } else {
                    tmp = tmp->left;
                }
            } else {
                if (tmp->right == NULL) {
                    tmp->right = node;
                    return;
                } else {
                    tmp = tmp->right;
                }
            }
        }
    }
    return;
}

Node* find(Tree* tree, int data) {
    if (tree->root == NULL) return tree->root;

    Node* tmp = tree->root;
    while (tmp != NULL) {
        if (data < tmp->data) {
            tmp = tmp->left;
        } else if (data > tmp->data) {
            tmp = tmp->right;
        } else {
            return tmp;
        }
    }
    return tmp;
}

void delete(Tree* tree, int data) {
    if (tree->root == NULL) return;
    Node* p = tree->root;    // p指向要删除的节点，初始化指向根节点
    Node* pp = NULL; // pp记录的是p的父节点

    while (p != NULL && p->data != data) {
        pp = p;
        if (data > p->data) p = p->right;
        else p = p->left;
    }

    if (p == NULL) return ; // 没有找到

    // 需要删除的节点有两个子节点
    if (p->left != NULL && p->right != NULL) {
        Node* minP = p->right;
        Node* minPP = p;    // minPP表示minP的父节点

        while (minP->left != NULL) {
            minPP = minP;
            minP = minP->left;
        }
        p->data = minP->data;    // 将minP的数据替换到p中
        p = minP;   // 下面就变成了删除minP了
        pp = minPP;
    }

    // 删除节点是叶子节点或者仅有一个子节点
    Node* child;    // p 的子节点
    if (p->left != NULL) child = p->left;
    else if (p->right != NULL) child = p->right;
    else child = NULL;

    /**
     * 删减具有左右节点是分两步
     * 1. 找到要删除节点的右子树的最小节点，然后将其值copy到要删除节点
     * 2. 删除右子树中的最小节点
     * 3. 将所有实际删除操作放到了最后
     */
    if (pp == NULL) tree->root = child;     // 删除的是根节点
    else if (pp->left == p) pp->left = child;
    else pp->right = child;
    free(p);

}

void inorder(Node* tree) {
    if (tree != NULL) {
        inorder(tree->left);
        printf("%d\n", tree->data);
        inorder(tree->right);
    }
}

void test_case_insert(Tree* tree) {
    int len = 15;
    int arr[] = {33, 16, 13, 18, 15, 17, 25, 19, 27, 50, 34, 58, 51, 66, 55};
    int i;
    for (i = 0; i < len; i++) {
        insert(tree, arr[i]);
    }
}

void test_case_search(Tree *tree) {
    int searchVal = 50;
    Node* retNode = find(tree, searchVal);
    if (retNode == NULL) {
        printf("查找失败\n");
        return;
    }

    printf("node val: %d\n", retNode->data);
    if (retNode->left != NULL) {
        printf("node left val:%d\n", retNode->left->data);   
    }

    if (retNode->right != NULL) {
        printf("node right val:%d\n", retNode->right->data);   
    }

}

void test_case_del(Tree *tree) {
    int deleteLen = 3;
    int deleteArr[] = {13, 18, 55};
    int i;

    for (i = 0; i < deleteLen; i++) {
        delete(tree, deleteArr[i]);
    }

    for (i = 0; i < deleteLen; i++) {
        Node* retNode = find(tree, deleteArr[i]);
        if (retNode == NULL) {
            printf("%d: 查找失败\n", deleteArr[i]);
            continue;
        }

        printf("%d | node val: %d\n", deleteArr[i], retNode->data);
        if (retNode->left != NULL) {
            printf("node left val:%d\n", retNode->left->data);   
        }

        if (retNode->right != NULL) {
            printf("%d | node right val:%d\n", deleteArr[i], retNode->right->data);   
        }
    }
    
}

int main() {
    Tree tree;
    tree.root = NULL;

    test_case_insert(&tree);
    test_case_search(&tree);
    printf("\n====== delete ======\n");
    test_case_del(&tree);

    return 0;
}

