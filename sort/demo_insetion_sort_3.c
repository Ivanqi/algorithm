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

struct ListNode* insertionSortList(struct ListNode* head){
    if (head == NULL) return head;
    struct ListNode *p = head;  // 内层循环
    struct ListNode *q = head;  // 外层循环
    struct ListNode *k = head;  // 记录点

    while (p->next != NULL) {
        // 如果next 比 当前val大，直接下一个循环
        if (p->next->val > p->val) {
            p = p->next;
            continue;
        }
        
        // 寻找插入位置
        if (p->next->val < q->val) {
            // 交换指针
            struct ListNode *temp = p->next;
            p->next = temp->next;
            temp->next = q;

            // 如果第一个指针比temp大，那么temp成为第一个指针
            if (q == head) {
                head = temp;
            } else {
                k->next = temp;
            }
            // 找到了。从头再来
            q = head;
        } else {
            // 如果没有找到插入位置
            if (p == q) {
                p = p->next;    // 外层循环
                continue;
            } else {
                k = q;          // 纪录q
                q = q->next;    // 内层循环
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
    struct ListNode *res = insertionSortList(n);
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
    struct ListNode *res = insertionSortList(n);
    printf("\n --------- \n");
    print(res);
}

int main() {

    test_case_2();
    return 0;
}