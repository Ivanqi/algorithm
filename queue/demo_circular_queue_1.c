#include <stdio.h>
#include <stdlib.h>
#include "demo_circular_queue_1.h"

#define bool int
#define true 1
#define false -1

circularQueue *create_queue(int size) {
    circularQueue *queue = (circularQueue *)malloc(sizeof(circularQueue));

    if (queue == NULL) {
        return NULL;
    }

    queue->max = size;
    queue->tail = 0;
    queue->head = 0;
    queue->array = (int *)malloc(sizeof(int) * size);

    if (queue->array == NULL) {
        return NULL;
    }

    return queue;
}

// 入队
bool enqueue(circularQueue *queue, int data) {
    if (queue == NULL) {
        return false;
    }

    if (_is_full(queue)) {
        return false;
    }

    queue->array[queue->tail] = data;
    queue->tail = (queue->tail + 1) % queue->max;
    return true;
}

// 出队
int dequeue(circularQueue *queue) {
    if (queue == NULL) {
        return false;
    }

    if (_is_empty(queue)) {
        return false;
    }

    int ret = queue->array[queue->head];
    queue->head = (queue->head + 1 ) % queue->max;
    
    return ret;
}

void queue_free(circularQueue *queue) {
    if (queue == NULL) {
        return;
    }

    if (queue->array == NULL) {
        return;
    }
    free(queue->array);
    free(queue);
}

void test_array_queue_1(circularQueue *queue, int size) {

    int i;
    for (i = 0 ; i < size; i++) {
        enqueue(queue, i);
    }

    
    // enqueue(queue, 3);

    for (i = 0 ; i < size; i++) {
        printf("dequeue val: %d\n", dequeue(queue));
    }
    enqueue(queue, 9);

    printf("dequeue val: %d\n", dequeue(queue));
    // printf("dequeue val: %d\n", dequeue(queue));
}

int main () {
    int size = 8;
    circularQueue *queue = create_queue(size);
    test_array_queue_1(queue, size);
    queue_free(queue);
    return 0;
}