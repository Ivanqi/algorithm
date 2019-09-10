#ifndef _LINK_QUEUE_
#define _LINK_QUEUE_

#define bool int
#define true 1
#define false -1
#define N 10

typedef struct LinkQueue {
    int key;
    struct LinkQueue *next;
}LinkQueue;

typedef struct List {
    int count;
    struct LinkQueue *head;
    struct LinkQueue *tail;
}List;

void create_queue(List *list);
bool enqueue(List *list, int data);
#endif