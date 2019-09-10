#ifndef _CIRCULAR_QUEUE_
#define _CIRCULAR_QUEUE_

typedef struct _circular_queue {
    int head;   // 队头下标
    int tail;   // 队尾下标
    int max;    // 队列最大容量
    int *array; // 数据存储区
}circularQueue;

#define _is_empty(circularQueue) (circularQueue->head == circularQueue->tail)
#define _is_full(circularQueue) ((circularQueue->tail + 1 ) % circularQueue->max == circularQueue->head)

#endif