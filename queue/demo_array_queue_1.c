#include <stdio.h>
#include <stdlib.h>
#include "demo_array_queue_1.h"

#define bool int
#define true 1
#define false -1

arrayQueue *create_queue(int size) {
    arrayQueue *queue = (arrayQueue *)malloc(sizeof(arrayQueue));

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
bool enqueue(arrayQueue *queue, int data) {
    if (queue == NULL) {
        return false;
    }

    if (_is_full(queue)) {
        if (queue->head == 0) return false;

        // 数据搬移
        int i;
        for (i = queue->head; i < queue->tail; ++i) {
            queue->array[i - queue->head] = queue->array[i];
        }
        // 搬移之后重新更新 head 和 tail
        queue->tail -= queue->head;
        queue->head = 0;
    }

    queue->array[queue->tail] = data;
    ++queue->tail;
    return true;
}

// 出队
int dequeue(arrayQueue *queue) {
    if (queue == NULL) {
        return false;
    }

    if (_is_empty(queue)) {
        return false;
    }

    int ret = queue->array[queue->head];
    ++queue->head;
    
    return ret;
}

void queue_free(arrayQueue *queue) {
    if (queue == NULL) {
        return;
    }

    if (queue->array == NULL) {
        return;
    }
    free(queue->array);
    free(queue);
}

void test_array_queue_1(arrayQueue *queue, int size) {

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
    arrayQueue *queue = create_queue(size);
    test_array_queue_1(queue, size);
    queue_free(queue);
    return 0;
}