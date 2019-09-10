#include <stdio.h>
#include <stdlib.h>
#include "demo_link_queue_1.h"

void create_queue(List *list) {
    list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

bool enqueue(List *list, int data) {

    LinkQueue *p = (LinkQueue *)malloc(sizeof(LinkQueue));
    p->key = data;
    p->next = NULL;

    if (list->head == NULL) {
        list->head = p;
    } else {
        list->tail->next = p;
    }
    list->tail = p;
    list->count++;
    return true;
}

int dequeue(List *list) {
    if (list == NULL || list->head == NULL) {
        return false;
    }
    
    LinkQueue *temp = list->head;
    int key = temp->key;
    list->head = temp->next;
    list->count--;
    if (list->head == NULL) {
        list->tail == NULL;
    }
    free(temp);
    return key;
}

void destory(List *list) {
    if (list == NULL || list->count == 0) return;
    while(!list->count == 0) {
        dequeue(list);
    }
    free(list);
}

void test_link_queue() {
    List l;
    create_queue(&l);
    int i;
    for (i = 0 ; i < 8; i++) {
        enqueue(&l, i);
    }
    for (i = 0 ; i < 8; i++) {
       printf("dequeue val: %d\n", dequeue(&l));
    }
    destory(&l);
}

int main () {
    test_link_queue();
    return 0;
}