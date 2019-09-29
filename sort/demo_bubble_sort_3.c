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

// 交换节点法
struct ListNode* bubbleSort(struct ListNode* head) {
    if (head == NULL) return head;
    struct ListNode *p; 
    struct ListNode *q;
    struct ListNode *tail;

    while ((head->next->next) != tail) {
        p = head;
        q = head->next;
        while (q->next != tail) {
            if ((q->val) > (q->next->val)) {
                p->next = q->next;
                q->next = q->next->next;
                p->next->next = q;
                q = p->next;
            }
            q = q->next;
            p = p->next;
        }
        tail = q;
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
    test_case_1();
    return 0;
}