#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo_link_queue_2.h"

listQueue *create_queue() {
    listQueue *queue = (listQueue *)malloc(sizeof(listQueue));
    if (queue == NULL) return NULL;

    queue->num = 0;
    queue->head = NULL;
    queue->tail = NULL;
}

void destroy_queue(listQueue *queue) {
    if ((queue == NULL) || (list_queue_is_empty(queue))) {
        return ;
    }

    while(!list_queue_is_empty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

bool enqueue(listQueue *queue, int data) {
    if (queue == NULL) return false;
    queueNnode *p = (queueNnode *)malloc(sizeof(queueNnode));
    if (p == NULL) return false;
    p->data = data;
    p->next = NULL;

    if (queue->head == NULL) {
        queue->head = p;
    } else {
        queue->tail->next = p;
    }
    queue->tail = p;
    queue->num++;

    return 0;
}

int dequeue(listQueue *queue) {
    if (queue == NULL) return -1;
    queueNnode *p = queue->head;
    int data = p->data;
    queue->head = queue->head->next;
    queue->num--;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(p);
    return data;
}


void test_link_queue(listQueue *queue) {

    int i;
    for (i = 0 ; i < 8; i++) {
        enqueue(queue, i);
    }
    
    for (i = 0 ; i < 8; i++) {
       printf("dequeue val: %d\n", dequeue(queue));
    }
}

int main () {
    listQueue *queue = create_queue();
    test_link_queue(queue);
    destroy_queue(queue);
    return 0;
}