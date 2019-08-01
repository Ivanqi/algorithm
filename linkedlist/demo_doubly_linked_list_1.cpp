#include <cstdio>
#include <cstdlib>
#include <cstring>

/**
 * 1. 双向链表基于单链表。单链表是单向的，有一个头结点，一个尾节点，要访问任何结点，都必须知道头结点，不能逆着进行
 * 2. 双链表添加一个指针域，通过两个指针域，分别指向结点的前结点和后结点
 * 3. 在双向链表中，结点除含有数据领外，还有两个链域，一个存储直接后继结点地址，一般称为右链域，一个存储直接前驱结点地址，一般称为左链域
 */

struct Node {
    int key;
    Node *prev, *next;
};

Node *nil;

void init() {
    nil = (Node *)malloc(sizeof(Node));
    nil->prev = nil;
    nil->next = nil;
}

void insert(int key) {
    Node *x = (Node *)malloc(sizeof(Node));
    x->key = key;
    x->next = nil->next;
    nil->next->prev = x;
    nil->next = x;
    x->prev = nil;
}

Node* search(int key) {
    Node *cur = nil->next;  // 头结点后面的元素开始访问
    while (cur != nil && cur->key != key) {
        cur = cur->next;
    }
    return cur;
}

void deleteNode(Node *t) {
    if (t == nil) return;   // t 为头结点不做处理
    t->prev->next = t->next;
    t->next->prev = t->prev;
    free(t);
}

void deleteKey(int key) {
    deleteNode(search(key));
}

void deleteFist() {
    deleteNode(nil->next);
}

void deleteLast() {
    deleteNode(nil->prev);
}

void print() {
    Node *cur = nil->next;
    int isf = 0;
    while (true) {
        if (cur == nil) break;
        if (isf++ > 0) printf(" ");
        printf("%d", cur->key);
        cur = cur->next;
    }
    printf("\n");
}





/***
 * 7
 * insert 5
 * insert 2
 * insert 3
 * insert 1
 * delete 3
 * insert 6
 * delete 6
 */
int main () {

    int key, n, i;
    int size = 0;
    char com[20];
    int np = 0, nd = 0;
    scanf("%d", &n);
    
    init();
    for (i = 0; i < n; i++) {
        scanf("%s%d", com, &key);   // 使用效率更高的输入函数
        if (com[0] == 'i') {
            insert(key);
            np++;
            size++;
        } else if (com[0] = 'd'){
            if (strlen(com) > 6) {
                if (com[6] == 'F') deleteFist();
                else if (com[6] == 'L') deleteLast();
            } else {
                deleteKey(key);
                nd++;
            }
            size--;
        }
    }
    print();
    return 0;
}


