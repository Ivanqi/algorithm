#include <stdlib.h>
#include <stdio.h>
#define random(x) (rand()%x)

typedef struct node {   // 树的结点
    int data;
    struct node* left;
    struct node* right;
} Node;

typedef struct {    // 树根
    Node* root;
}Tree;

void insert(Tree* tree, int value)  // 创建树
{
    Node* node = (Node*)malloc(sizeof(Node));   // 创建一个节点
    node->data = value;
    node->left = NULL;
    node->right = NULL;

    if (tree->root == NULL) {   // 判读树是不是空树
        tree->root = node;
    } else {    // 不是空树
        Node* temp = tree->root;    // 从树根开始
        while (temp != NULL) {
            if (value < temp->data) {
                if (temp->left == NULL) {
                    temp->left = node;
                    return;
                } else {
                    temp = temp->left;
                }
            } else {
                if (temp->right == NULL) {
                    temp->right = node;
                    return ;
                } else {
                    temp = temp->right;
                }
            }
        }
    }
    return;
}

void inorder(Node* node) {  // 树的中序遍历
    if (node != NULL) {
        inorder(node->left);
        printf("%d\n", node->data);
        inorder(node->right);
    }
}

int main() {

    Tree tree;
    tree.root = NULL;   // 创建一个空树
    int n = 7;
    int i;
    
    for (i = 0; i < n; i++) {
        int temp = random(20);
        insert(&tree, temp);
    }

    inorder(tree.root);
    return 0;
}