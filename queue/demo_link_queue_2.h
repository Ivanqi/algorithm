#ifndef LINK_LIST_QUEUE_H
#define LINK_LIST_QUEUE_H

#define bool int
#define true 1
#define false -1


typedef struct _list_queue_node
{
    int data;
    struct _list_queue_node *next;
}queueNnode;

typedef struct _list_queue
{
    int num;
    queueNnode *head;
    queueNnode *tail;
}listQueue;

#define list_queue_is_empty(queue) ((queue->num) == 0)

#endif