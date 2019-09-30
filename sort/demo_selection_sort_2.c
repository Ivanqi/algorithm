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

/**
 * 1. 先在原链表中找最小的，找到一个后就把它放到另一个空的链表中
 * 2. 空链表中安放第一个进来的节点，产生一个有序链表，并且让它在原链表中分离出来(此时要注意原链表中出来的第一个节点还是中间其他节点)
 * 3. 继续在原链表中找下一个最小的，找到后把它放入有序链表的尾指针的next，然后它变成其尾指针
 */
struct ListNode* selectionSort(struct ListNode* head) {
    struct ListNode *first; // 排列后有序链的表头指针
    struct ListNode *tail;  // 排列后有序链的表尾指针
    struct ListNode *p_min; // 保留键值更小的节点的前驱节点的指针
    struct ListNode *min;   // 存储最小节点
    struct ListNode *p;     // 当前比较节点

    first = NULL;
    while (head != NULL) {  // 在链表中找键值最小的节点
        // 注意：这里for语句就是体现选择排序的思想的地方
        for (p = head, min = head; p->next != NULL; p = p->next) {  // 循环遍历链表中的节点，找出此时最小的节点
            if (p->next->val < min->val) {  // 找到一个比当前min小的节点
                p_min = p;      // 保存找到节点的前驱节点：显然 p->next 的前驱节点是p
                min = p->next;  // 保存键值更小的节点
            }
        }
        /**
         * 上面for 语句结束后，就要做两件事情
         *  1. 一是把它放入有序链表中
         *  2. 根据相应的条件判断，安排它离开原来的链表
         */
        // 第一件事
        if (first == NULL) {    // 如果有序链表目前还是一个空链表
            first = min;        // 第一次找到键值最小的节点
            tail = min;         // 注意：尾指针让它指向最后一个节点
        } else {                // 有序链表中已经有节点
            tail->next = min;   // 把刚找到的最小节点放到最后，即让尾指针的next指向他
            tail = min;         // 尾指针也要指向它
        }

        // 第二件事
        if (min == head) {      // 如果找到的最小节点就是第一个节点
            head = head->next;  // 显然让head指向原head->next， 即第二个节点，就OK
        } else {                // 如果不是第一个节点
            p_min->next = min->next;    // 前次最小节点的next指向当前min的next，这样让min离开了原链表
        }
    }
    if (first != NULL) {    // 循环结束得到有序链表 first
        tail->next = NULL;
    }

    head = first;
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
    struct ListNode *res = selectionSort(n);
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
    struct ListNode *res = selectionSort(n);
    printf("\n --------- \n");
    print(res);
}

int main() {
    test_case_1();
    return 0;
}