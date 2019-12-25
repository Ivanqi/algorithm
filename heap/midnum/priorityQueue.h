#ifndef _PRIORITY_QUEUE_
#define _PRIORITY_QUEUE_
#define MAX_HEAP_SIZE 255

typedef struct heap {
    int count;
    int capacity;
    int (*cmp)(int i, int j);
    int *array; 
} Heap;


typedef int (*cmp_func)(int i, int j);

Heap* heap_create(int capacity, cmp_func cmp);
void heap_insert(Heap *heap, int data);
int heap_pop(Heap *heap);
void heap_heapify(Heap *heap, int n, int parent);
void heap_swap(int *array, int a, int b);
void heap_destory(Heap *heap);
int heap_empty(Heap *heap);
int heap_top(Heap *heap);

#endif