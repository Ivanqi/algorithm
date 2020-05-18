#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node
{
    int value;
    struct _Node* next;
} Node;

typedef struct _LinkList
{
    int len;
    Node* head;
} LinkList;

LinkList* createList()
{
    LinkList* list = (LinkList*)malloc(sizeof(LinkList));
    list->len = 0;
    list->head = NULL;

    return list;
}

// 尾插
void insertTail(LinkList* list, int value)
{
    Node* n = (Node *)malloc(sizeof(Node));
    n->value = value;
    n->next = NULL;

    list->len++;

    if (list->head == NULL) {
        list->head = n;
    }

    Node *cur = list->head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = n;
}

// 降序
void insertSorted(LinkList* list, int value)
{
    Node* n = (Node*)malloc(sizeof(Node));
    n->value = value;
    n->next = NULL;

    list->len++;

    Node* prev = NULL;
    Node* cur = list->head;

    while (cur != NULL) {
        if (cur->value < value) break;

        prev = cur;
        cur = cur->next;
    }

    if (prev) {
        prev->next = n;
        n->next = cur;
    } else {
        n->next = cur;
        list->head = n;
    }
}

void insertTail2(LinkList* list, int value)
{
    Node* n = (Node*)malloc(sizeof(Node));
    n->value = value;
    n->next = NULL;

    list->len++;

    Node** cur = &list->head;
    
    while (*cur) {
        /**
         * 1. 计算*cur
         *  1. *cur 获取二级指针指向链表指针，该指针和普通指针一样，指向了下一个节点
         * 2. 计算(*cur)->next
         *  1. 获取下一个节点的next指针
         * 3. 计算&(*cur)->next
         *  1. 获取next指针的地址，然后将其赋给新的cur二级指针，如此便完成了基于二级指针的链表遍历
         */
        cur = &(*cur)->next;
    }
    // *cur获取的是当前最后一个节点的next指针，将该next赋值也就是使其指向了下一个节点
    *cur = n;
}

void insertSorted2(LinkList* list, int value)
{
    Node* n = (Node*)malloc(sizeof(Node));
    n->value = value;

    list->len++;

    Node** cur = &list->head;

    while (*cur) {
        if ((*cur)->value < value) break;
        cur = &(*cur)->next;
    }

    n->next = *cur;
    *cur = n;
}

LinkList* copyList(LinkList* list)
{
    LinkList* newList = createList();

    Node* cur = list->head;
    Node* newCur = newList->head;

    while (cur)
    {
        Node* n = (Node*) malloc(sizeof(Node));
        n->value = cur->value;

        if (newCur != NULL) {
            newCur->next = n;
        } else {
            newList->head = n;
        }

        newCur = n;
        cur = cur->next;
    }

    newCur->next = NULL;
    newList->len = list->len;

    return newList;
}

LinkList* copyList2(LinkList* list)
{
    LinkList* newList = createList();

    Node* cur = list->head;
    Node** newCur = &newList->head;

    while (cur) {
        Node* n = (Node*)malloc(sizeof(Node));
        n->value = cur->value;

        *newCur = n;
        newCur = &n->next;

        cur = cur->next;
    }

    *newCur = NULL;
    newList->len = list->len;

    return newList;
}

void deleteValue(LinkList* list, int value)
{
    Node* prev = NULL;
    Node* cur = list->head;

    while (cur != NULL) {
        Node* n = cur->next;

        if (cur->value == value) {
            if (prev) {
                prev->next = n;
            } else {
                list->head = n;
            }
            free(cur);
        } else {
            prev = cur;
        }

        cur = n;
    }
}

void deleteValue2(LinkList* list, int value)
{
    Node** cur = &list->head;

    while (*cur != NULL) {
        Node* entry = *cur;

        if (entry->value == value) {
            *cur = entry->next;
            free(entry);
        } else {
            cur = &entry->next;
        }
    }
}

void test_case_insert(LinkList *list) {
    int i;
    for (i = 0; i < 50; i++) {
        if (i % 3 != 0) continue;
        insertTail2(list, i);
    }

    for (i = 0; i < 50; i++) {
        if (i % 2 == 0) {
            insertSorted2(list, i);
        }
    }
}

void test_case_delete(LinkList *list) {
    int deleteNum = 10;
    deleteValue2(list, deleteNum);
}

int main() {

    LinkList *list = createList();
    test_case_insert(list);
    test_case_delete(list);
}