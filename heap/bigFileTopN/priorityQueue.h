#ifndef _PRIORITY_QUEUE_
#define _PRIORITY_QUEUE_
#define MAX_HEAP_SIZE 255

typedef struct heap_info {
    void *key;
    int num;
} HeapInfo;

typedef struct heap {
    int count;
    int capacity;
    int (*cmp)(char *key1, char *key2);
    HeapInfo **array; 
} Heap;


typedef int (*cmp_func)(char *key1, char *key2);

Heap* heap_create(int capacity, cmp_func cmp);
void heap_insert(Heap *heap, char *key, int num);
HeapInfo* heap_top(Heap *heap);
void heap_heapify(Heap *heap, int n, int parent);
void heap_swap(HeapInfo **array, int i, int j);
void heap_destory(Heap *heap);
int heap_empty(Heap *heap);
HeapInfo* heap_pop(Heap *heap);


static inline int min_keycmp(char *a, char *b) {
    if (strcmp(a, b) < 0) return 1;
    else return 0;
}

static inline int max_keycmp(char *a, char *b) {
    if (strcmp(a, b) > 0) return 1;
    else return 0;
}

static inline int min_cmp(int a, int b) {
    if (a < b) return 1;
    else return 0;
}

static inline int max_cmp(int a, int b) {
    if (a > b) return 1;
    else return 0;
}

#endif