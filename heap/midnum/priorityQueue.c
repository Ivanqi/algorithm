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

    tmp->array = (int*)malloc(sizeof(int) * capacity);

    assert(tmp->array != NULL);

    return tmp;
}

void heap_insert(Heap *heap, int data) {

    assert(heap != NULL);
    if (heap->count >= heap->capacity) {
        printf("数据已满，无法插入\n");
        return;
    }

    heap->count++;
    heap->array[heap->count] = data;
   

    int i = heap->count;
    int parent = i / 2;
    while (parent > 0 && heap->cmp(heap->array[i], heap->array[parent])) {    // 自下往上堆化
        heap_swap(heap->array, i , parent);
        i = parent;
    }
}

void heap_swap(int *array, int i, int j) {
    int tmp;
    tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}


int heap_pop(Heap *heap) {
    assert(heap != NULL);
    if (heap->count == 0) {
        printf("没有数据");
    }
    int tmp = heap->array[1];
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

        if (left <= n && !heap->cmp(heap->array[parent], heap->array[left])) maxPos = left;
        if (right <= n && !heap->cmp(heap->array[maxPos], heap->array[right])) maxPos = right;

        if (maxPos == parent) break;
        heap_swap(heap->array, parent, maxPos);
        parent = maxPos;
    }
}

int heap_empty(Heap *heap) {
    assert(heap != NULL);
    return (heap->count == 0 ? 1 : 0);
}

int heap_top(Heap *heap) {
    return heap->array[1];
}

void heap_destory(Heap *heap) {
    assert(heap != NULL);

    if (heap->array != NULL) {
        free(heap->array);
    }
    free(heap);
}
