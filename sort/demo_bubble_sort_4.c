#include <stdlib.h>
#include <stdio.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* insert(struct ListNode *node, int val) {
    struct ListNode *newNode = NULL;
    newNode = (struct ListNode *) malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = node;
    node = newNode;
    return node;
}

// 节点值交换法
struct ListNode* bubbleSort(struct ListNode* head) {
    if (head == NULL) return head;
    struct ListNode *p;         // 外层循环
    struct ListNode *q;         // 内层循环

    for (p = head; p != NULL; p = p->next) {
        for (q = p->next; q != NULL; q = q->next) {
            if ((p->val) > (q->val)) {
                int s = q->val;
                q->val = p->val;
                p->val = s;
            }
        }
    }
    return head;
}

void print(struct ListNode *node){
    if (node == NULL) {
        exit(1);
    }
    struct ListNode *p = node;
    while(p != NULL) {
        printf("Node val: %d\n", p->val);
        p = p->next;
    }
}

void test_case_1() {
    struct ListNode *n = NULL;
    int arr[] = {4, 2, 1, 3};
    int i;
    for (i = 0; i < 4; i++) {
        n = insert(n, arr[i]);
    }
    print(n);
    struct ListNode *res = bubbleSort(n);
    printf("\n --------- \n");
    print(res);
}

void test_case_2() {
    struct ListNode *n = NULL;
    int arr[] = {-1, 5, 3, 4, 0};
    int i;
    for (i = 0; i < 5; i++) {
        n = insert(n, arr[i]);
    }
    print(n);
    struct ListNode *res = bubbleSort(n);
    printf("\n --------- \n");
    print(res);
}

int main() {
    test_case_2();
    return 0;
}