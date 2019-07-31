#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

struct Node {
    int key;
    Node *right, *left, *parent;
};

Node *root, *NIL;

Node * treeMinimum (Node *x) {
    while (x->left != NIL) x = x->left;
    return x;
}

Node * find (Node *u, int k) {

    while (u != NIL && u->key != k) {
        if (k < u->key) {
            u = u->left;
        } else {
            u = u->right;
        }
    }
    return u;
}

Node * treeSuccessor(Node *x) {
    if (x->right != NIL) return treeMinimum(x->right);
    Node *y = x->parent;
    while (y != NIL && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

void treeDelete(Node *z) {

    Node *y;        // 要删除的对象
    Node *x;        // y的子节点

    // 确定要删除的结点
    if (z->left == NIL || z->right == NIL) y = z;
    else y = treeSuccessor(z);

    // 确定y的子结点x
    if (y->left != NIL) {
        x = y->left;
    }  else {
        x = y->right;
    }

    if (x != NIL) {
        x->parent = y->parent;
    }

    if (y->parent == NIL) {
        root = x;
    } else {
        if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    }

    if ( y != z ) {
        z->key = y->key;
    }

    free(y);
}

void insert (int k) {

    Node *y = NIL;
    Node *x = root;
    Node *z;

    z = (Node *)malloc(sizeof(Node));
    z->key = k;
    z->left = NIL;
    z->right = NIL;

    while (x != NIL) {
        y = x;
        if (k < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;

    if (y == NIL) {
        root = z;
    } else {
        if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }
    }
}

void inorder(Node *u) {
    if (u == NIL) return;
    inorder(u->left);
    printf(" %d", u->key);
    inorder(u->right);
}

void preorder(Node *u) {
    if (u == NIL) return;
    printf(" %d", u->key);
    inorder(u->left);
    inorder(u->right);
}


int main () {

    int n, i, x;
    string com;

    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        cin >> com;
        if (com[0] == 'f') {
            scanf("%d", &x);
            Node *t = find(root, x);
            if (t != NIL) printf("yes\n");
            else printf("no\n");
        } else if (com == "insert") {
            scanf("%d", &x);
            insert(x);
        } else if (com == "print") {
            inorder(root);
            printf("\n");
            preorder(root);
            printf("\n");
        } else if (com == "delete") {
            scanf("%d", &x);
            treeDelete(find(root, x));
        }
    }

    return 0;
}