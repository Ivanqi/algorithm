#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <priorityQueue.h>

MinHeap* heap_create(int capacity, FILE *output, keycmp_func keycmp, heap_remove_top_func remove_func) {
    MinHeap* tmp = (MinHeap *)malloc(sizeof(MinHeap));
    tmp->count = 0;
    tmp->capacity = capacity;
    tmp->keycmp = keycmp;
    tmp->remove_func = remove_func;
    tmp->output = output;

    tmp->heap_info = (HeapInfo**)malloc(sizeof(HeapInfo*) * capacity);

    assert(tmp->heap_info != NULL);

    return tmp;
}

void heap_insert(MinHeap *heap, FileManage *fm) {

    assert(heap != NULL);
    assert(fm != NULL);
    if (heap->count >= heap->capacity) {
        printf("数据已满，无法插入\n");
        return;
    }


    HeapInfo *tmp = (HeapInfo *) malloc(sizeof(HeapInfo));
    tmp->file_info = fm;
    tmp->val = fm->buf[fm->pos];
    fm->pos = 0;

    heap->count++;
    heap->heap_info[heap->count] = tmp;
   

    int i = heap->count;
    while (i / 2 > 0 && heap->keycmp(&heap->heap_info[i]->val, &heap->heap_info[i / 2]->val)) {    // 自下往上堆化
        heap_swap(heap->heap_info, i , i / 2);
        i = i / 2;
    }
}

void heap_swap(HeapInfo **info, int i, int j) {
    HeapInfo *tmp;
    tmp = info[i];
    info[i] = info[j];
    info[j] = tmp;
}


void heap_remove_top(MinHeap *heap) {
    assert(heap != NULL);
    if (heap->count == 0) {
        printf("没有数据");
    }
    if (!heap->remove_func(heap->output, heap->heap_info[1])) {
        heap->heap_info[1] = heap->heap_info[heap->count];
        --heap->count;
    }
    heap_heapify(heap, heap->count, 1);
}

void heap_heapify(MinHeap *heap, int n, int parent) { // 自上而下堆化
    int maxPos , left, right;

    while (1) {
        maxPos = parent;
        left = parent * 2;
        right = left + 1;

        if (left <= n && !heap->keycmp(&heap->heap_info[parent]->val, &heap->heap_info[left]->val)) maxPos = left;
        if (right <= n && !heap->keycmp(&heap->heap_info[maxPos]->val, &heap->heap_info[right]->val)) maxPos = right;

        if (maxPos == parent) break;
        heap_swap(heap->heap_info, parent, maxPos);
        parent = maxPos;
    }
}

void heap_destory(MinHeap *heap) {
    assert(heap != NULL);

    if (heap->heap_info != NULL) {
        int i;
        for (i = 0; i <= heap->count; i++) {
            HeapInfo *tmp = heap->heap_info[0];
            if (tmp != NULL && tmp->file_info != NULL) {
                free(tmp->file_info);
            }
            free(tmp);
        }
    }
    free(heap);
}