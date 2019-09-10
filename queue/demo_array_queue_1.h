#ifndef _ARRARY_QUEUE_
#define _ARRARY_QUEUE_

typedef struct _array_queue {
    int head;   // 队头下标
    int tail;   // 队尾下标
    int max;    // 队列最大容量
    int *array; // 数据存储区
}arrayQueue;

#define _is_empty(arrayQueue) (arrayQueue->head == arrayQueue->tail)
#define _is_full(arrayQueue) (arrayQueue->tail == arrayQueue->max)

#endif