#include <stdio.h>
#include <stdlib.h>

#define bool int
#define true 1
#define false 0

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;


ListNode* insert(ListNode *node, int val) {
    ListNode *newNode = NULL;
    newNode = (ListNode *) malloc(sizeof(ListNode));
    newNode->val = val;
    newNode->next = node;
    node = newNode;

    return node;
}

int search(ListNode *node, int val) {
    if (node == NULL) {
        exit(1);
    }
    ListNode *p = node;
    int counter = 0;
    while (p != NULL && p->val != val) {
        p = p->next;
        counter++;
    }

    return counter;
}

ListNode* del(ListNode *node, int val)
{
    if (node == NULL) {
        exit(1);
    }

    ListNode *p = node;
    ListNode *tmp;
    while (p != NULL && p->next->val != val) {
        p = p->next;
    }

    tmp = p->next;
    p->next = tmp->next;
    return node;
}

// 反转指针
ListNode* reverse(ListNode* node)
{
    if (node == NULL) {
        exit(1);
    }
    ListNode* pNode = node;
    ListNode* pNext = NULL;
    ListNode* pPrev = NULL;
    ListNode* preReverseHead = NULL;    // 新链表头指针

    while (pNode != NULL) {
        pNext = pNode->next;
        if (pNext == NULL) {
            preReverseHead = pNode;
        }
        pNode->next = pPrev;           // 指针反转
        pPrev = pNode;
        pNode = pNext;
    }
    return preReverseHead;
}

/**
 * 判断是否是回文字符串
 *  1. 使用快慢两个指针找到链表中点，慢指针每次前进一步，快指针每次前进两步
 *  2. 在慢指针前进的过程中，同时修改其next指针，使得链表前半部分反序
 *  3. 最后比较中点两侧链表是否相等
 */

bool isPalindrome(ListNode* node)
{
    if (node == NULL && node->next == NULL) {
        exit(1);
    }
    ListNode *reverse = NULL;
    ListNode *slow = node;
    ListNode *fast = node;

    while (slow && fast) {              // 获取到中间结点
        slow = slow->next;
        fast = fast->next ? fast->next->next : fast->next;
    }

    while(slow) {              // 反转指针
        ListNode *tmpNext = slow->next;
        slow->next = reverse;
        reverse = slow;
        slow = tmpNext;
    }

    while (node && reverse) {
        if (node->val != reverse->val) {
            return false;
        }
        node = node->next;
        reverse = reverse->next;
    }

    return true;
}

/**
 * 链表有环判断
 *  1. 定义两个指针。fast 和 slow 。最初的时候，fast和slow都指向链表的初始结点head。然后每一次操作fast向前走两步。slow向前走一位
 *  2. 因为fast 比 slow移动快，如果有环，那么fast一定会先进入环，而slow后进入环
 *  3. 当两个指针都进入环后，经过一定次数的操作，fast和slow最终会在环上相遇，并且一定是在slow绕环走完一圈之前相遇
 */

bool hasCycel(ListNode *node)
{
    if (node == NULL) {
        exit(1);
    }
    ListNode *slow = node, *fast = node;

    while(slow && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}


// 中间结点
ListNode* middleNode(ListNode* node)
{
    if (node == NULL) {
        exit(1);
    }
    ListNode *slow = node, *fast = node;
    while(slow && fast->next) {
        slow = slow->next;
        fast = fast->next ? fast->next->next: fast->next;
    }
    return slow;
}

// 合并两个有序链表
ListNode* mergeTwoLists(ListNode *l1, ListNode *l2)
{
    if (l1 == NULL || l2 == NULL) {
        exit(1);
    }
    ListNode *p = insert(NULL, 0);
    ListNode *curr = p;
    while (l1 && l2) {
        if (l1->val > l2->val ) {
            curr->next = l1;
            curr = curr->next;
            l1 = l1->next;
        } else if (l1->val < l2->val) {
            curr->next = l2;
            curr = curr->next;
            l2 = l2->next;
        } else {
            curr->next = l1;
            curr = curr->next;
            l1 = l1->next;
            l2 = l2->next;   
        }
    }

    if (l1 == NULL) {
        curr->next = l2;
    } else {
        curr->next = l1;
    }
    return p->next;
}

// 删除链表倒数第 n 个结点
ListNode* removeNthFromEnd(ListNode* node, int n)
{
    if (node == NULL) {
        exit(1);
    }

    ListNode *slow = node, *fast = node;
    int i;
    for (i = 0; i < n; i++) {
        fast = fast->next;
    }
    if (fast == NULL) return node->next;
    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }

    ListNode *tmp;
    tmp = slow->next;
    slow->next = tmp->next;
    return node;
}


void print(ListNode *node){
    if (node == NULL) {
        exit(1);
    }
    ListNode *p = node;
    while(p != NULL) {
        printf("Node val: %d\n", p->val);
        p = p->next;
    }
}

void test_insert() {
    ListNode *n = NULL;
    n = insert(n, 1);
    n = insert(n, 2);
    print(n);
}

void test_search() {
    ListNode *n = NULL;
    int arr[] = {1, 2, 3, 5, 3, 2, 1};
    int i;
    for (i = 0; i < 7; i++) {
        n = insert(n, arr[i]);
    }
    print(n);

    int index = search(n, 5);
    printf("search 5 - index: %d\n", index);
    
}

void test_del() {
    ListNode *n = NULL;
    int arr[] = {1, 2, 3, 5, 3, 2, 1};
    int i;
    for (i = 0; i < 7; i++) {
        n = insert(n, arr[i]);
    }
    print(n);

    printf("delete 3\n");
    n = del(n, 3);
    print(n);
}

void test_reverse() {
    ListNode *n = NULL;
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int i;
    for (i = 0; i < 7; i++) {
        n = insert(n, arr[i]);
    }
    print(n);
    printf("reverse link\n");
    n = reverse(n);
    print(n);
}

void test_isPalindrome()
{
    ListNode *n1 = NULL;
    ListNode *n2 = NULL;
    ListNode *n3 = NULL;

    int arr1[] = {1, 2, 3, 4, 5, 6, 7};
    int arr2[] = {1, 2, 3, 5, 3, 2, 1};
    int arr3[] = {1, 2, 3, 3, 2, 1};
    int i;

    for (i = 0; i < 7; i++) {
        n1 = insert(n1, arr1[i]);
    }

    for (i = 0; i < 7; i++) {
        n2 = insert(n2, arr2[i]);
    }

    for (i = 0; i < 6; i++) {
        n3 = insert(n3, arr3[i]);
    }
    printf("\nn1 check\n");
    print(n1);

    if (isPalindrome(n1)) {
        printf("是回文字符串");
    } else {
        printf("不是回文字符串");
    }

    printf("\n\nn2 check\n");
    print(n2);
    if (isPalindrome(n2)) {
        printf("是回文字符串\n");
    } else {
        printf("不是回文字符串\n");
    }

    printf("\n\nn3 check\n");
    print(n3);
    if (isPalindrome(n3)) {
        printf("是回文字符串\n");
    } else {
        printf("不是回文字符串\n");
    }
}

void test_hasCycel()
{
    ListNode *n = NULL;
    n = insert(n, 1);
    n = insert(n, 2);
    n = insert(n, 3);
    
    printf("result1: ");
    if (hasCycel(n)) {
        printf("链表有环\n");
    } else {
        printf("链表无环\n");
    }
    printf("\n");

    printf("result2: ");
    
    ListNode *current = (ListNode* )malloc(sizeof(ListNode));
    current->val = 0;
    ListNode *h = current;
    int i;
    for (i = 0; i < 4; i++) {
        ListNode *node = (ListNode *)malloc(sizeof(ListNode));
        node->val = i;
        current->next = node;
        current = node;
    }
    current->next = h;

    if (hasCycel(current)) {
        printf("链表有环\n");
    } else {
        printf("链表无环\n");
    }
}

void test_middleNode()
{
    ListNode *n1 = NULL;
    ListNode *n2 = NULL;
    int arr1[] = {1, 2, 3, 4, 5, 6, 7};
    int arr2[] = {1, 2, 3, 3, 2, 1};
    int i;
    for (i = 0; i < 7; i++) {
        n1 = insert(n1, arr1[i]);
    }

    for (i = 0; i < 6; i++) {
        n2 = insert(n2, arr2[i]);
    }

    print(n1);
    ListNode *middle = middleNode(n1);
    printf("get n1 middleNode: %d\n", middle->val);

    printf("\n");
    print(n2);
    middle = middleNode(n2);
    printf("get n2 middleNode: %d\n", middle->val);
}

void test_mergeTwoLists()
{
    ListNode *n1 = NULL;
    ListNode *n2 = NULL;

    n1 = insert(n1, 3);
    n1 = insert(n1, 5);
    n2 = insert(n2, 1);
    n2 = insert(n2, 2);
    n2 = insert(n2, 4);
    n2 = insert(n2, 6);
    n2 = insert(n2, 7);

    ListNode *n = mergeTwoLists(n1, n2);
    print(n);
}

void test_removeNthFromEnd()
{
    ListNode *n = NULL;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int i;
    for (i = 0; i < 9; i++) {
        n = insert(n, arr[i]);
    }
    print(n);
    printf("removeNthFromEnd: 3\n");
    n = removeNthFromEnd(n, 3);
    print(n);
}

int main () {

    test_removeNthFromEnd();
    return 0;
}

