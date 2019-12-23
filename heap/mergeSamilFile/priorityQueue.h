#ifndef _PRIORITY_QUEUE_
#define _PRIORITY_QUEUE_
#define MAX_HEAP_SIZE 255
#include "fileManage.h"

typedef struct heap_info {
    char val[1];
    FileManage *file_info;
} HeapInfo;


typedef struct min_heap {
    int count;
    int capacity;
    int (*remove_func)(FILE *output, HeapInfo* heap_info);
    int (*keycmp)(char *key1, char *key2);
    FILE *output;
    HeapInfo **heap_info; 
} MinHeap;


typedef int (*heap_remove_top_func)(FILE *output, HeapInfo* heap_info);
typedef int (*keycmp_func)(char *key1, char *key2);

MinHeap* heap_create(int capacity, FILE *output, keycmp_func keycmp, heap_remove_top_func remove_func);
void heap_insert(MinHeap *heap, FileManage *fm);
void heap_remove_top(MinHeap *heap);
void heap_heapify(MinHeap *heap, int n, int parent);
void heap_swap(HeapInfo **info, int a, int b);
void heap_destory(MinHeap *heap);

#endif