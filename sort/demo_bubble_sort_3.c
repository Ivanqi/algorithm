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

struct ListNode* bubbleSort(struct ListNode* head) {
    struct ListNode* tail = NULL;
    head = insert(head, -999999);

    while(tail != head->next) {
        struct ListNode* pre = head;
        struct ListNode* cur = pre->next;
        while(cur != tail && cur->next != tail) {
            if( cur->val > cur->next->val ) {   
                //交换当前节点和后一个节点
                pre->next = cur->next;
                cur->next = cur->next->next;
                pre->next->next = cur;
            }
            pre = pre->next;
            cur = pre->next;
        }
 
        tail = cur;
    }
    return head->next;
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