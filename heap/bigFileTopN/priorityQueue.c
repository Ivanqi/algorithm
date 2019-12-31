#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <priorityQueue.h>

Heap* heap_create(int capacity, cmp_func cmp) {
    Heap* tmp = (Heap *)malloc(sizeof(Heap));
    assert(tmp != NULL);
    tmp->count = 0;
    tmp->capacity = capacity;
    tmp->cmp = cmp;


    tmp->array = (HeapInfo**)malloc(sizeof(HeapInfo *) * capacity);

    assert(tmp->array != NULL);

    return tmp;
}

void heap_insert(Heap *heap, char *key, int num) {

    assert(heap != NULL);
    if (heap->count >= heap->capacity) {
        heap_pop(heap);
    }

    HeapInfo *data = (HeapInfo*)malloc(sizeof(HeapInfo));
    data->key = key;
    data->num = num;
    heap->count++;
    heap->array[heap->count] = data;
   

    int i = heap->count;
    while (i / 2 > 0 && heap->cmp(heap->array[i]->key, heap->array[i / 2]->key)) {    // 自下往上堆化
        heap_swap(heap->array, i , i / 2);
        i = i / 2;
    }
}

void heap_swap(HeapInfo **array, int i, int j) {
    HeapInfo *tmp;
    tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}


HeapInfo* heap_pop(Heap *heap) {
    assert(heap != NULL);
    if (heap->count == 0) {
        printf("没有数据");
    }
    HeapInfo *tmp = heap->array[1];
    heap->array[1] = heap->array[heap->count];
    --heap->count;
    heap_heapify(heap, heap->count, 1);
    return tmp;
}

void heap_heapify(Heap *heap, int n, int parent) { // 自上而下堆化
    int maxPos , left, right;

    while (1) {
        maxPos = parent;
        left = parent * 2;
        right = left + 1;

        if (left <= n && !heap->cmp(heap->array[parent]->key, heap->array[left]->key)) maxPos = left;
        if (right <= n && !heap->cmp(heap->array[maxPos]->key, heap->array[right]->key)) maxPos = right;

        if (maxPos == parent) break;
        heap_swap(heap->array, parent, maxPos);
        parent = maxPos;
    }
}

int heap_empty(Heap *heap) {
    assert(heap != NULL);
    return (heap->count == 0 ? 1 : 0);
}

HeapInfo* heap_top(Heap *heap) {
    return heap->array[1];
}

void heap_destory(Heap *heap) {
    assert(heap != NULL);

    if (heap->array != NULL) {
        free(heap->array);
    }
    free(heap);
}