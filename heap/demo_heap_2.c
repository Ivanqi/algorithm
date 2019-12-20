#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct heap {
    int capacity;
    int count;
    int *arr;
} heap;

heap* heap_create(int capacity);
void swap(int *arr, int i, int j);
void insert(heap *h, int data);
void removeMax(heap *h);
void heapify(heap *h, int n, int parent);
void buildHeap(heap *h);
void sort (heap *h);

void swap(int *arr, int i, int j) {
    int temp;
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

heap* heap_create(int capacity) {
    assert(capacity >= 1);
    heap *h = (heap*)malloc(sizeof(heap));
    assert(h != NULL);
    if (h != NULL) {
        h->capacity = capacity;
        h->count = 0;
        h->arr = (int*)malloc(sizeof(int) * capacity);
        assert(h->arr != NULL);
    }

    return h;
}

void insert(heap *h, int data) {
    assert(h != NULL);
    if (h->count >= h->capacity) {
        printf("数据已满\n");
        return;
    }

    h->count++;
    h->arr[h->count] = data;
    int i = h->count;

    while (i / 2 > 0 && h->arr[i] > h->arr[i / 2]) {  // 自下往上堆化
        swap(h->arr, i, i / 2);
        i = i / 2;
    }
}

void removeMax(heap *h) {
    assert(h != NULL);
    if (h->count == 0) {
        printf("没有数据\n");
        return;
    }
    h->arr[1] = h->arr[h->count];
    --h->count;
    heapify(h, h->count, 1);
}

void heapify(heap *h, int n, int parent) { // 自上往下堆化
    int maxPos, left, right;

    while (1) {
        maxPos = parent;
        left = parent * 2;
        right = left + 1;

        if (left <= n && h->arr[parent] < h->arr[left]) maxPos = left;
        if (right <= n && h->arr[maxPos] < h->arr[right]) maxPos = right;


        if (maxPos == parent) break;
        swap(h->arr, parent, maxPos);
        parent = maxPos;
    }
}

void buildHeap(heap *h) {
    assert(h != NULL);
    for (int i = h->count / 2; i >= 1; --i) {
        heapify(h, h->count, i);
    }
}

void sort (heap *h) {
    assert(h != NULL);
    int k = h->count;
    while (k > 1) {
        swap(h->arr, 1, k);
        --k;
        heapify(h, k, 1);
    }
}

void destroy(heap *h) {
    assert(h != NULL);
    free(h);
}

void test_case_1() {
    heap *h;
    int capacity = 10;
    h = heap_create(capacity);

    int i;
    int max = 12;
    for (i = 1; i < max; i++) {
        if (i % 2 == 0) {
            insert(h, i * 10);
        } else {
            insert(h, i);
        }
    }
    

    for (i = 1; i <= h->count; i++) {
        printf("%d\t", h->arr[i]);
    }
    printf("\n");

    removeMax(h);
    printf("----- delete ---- \n");
    for (i = 1; i <= h->count; i++) {
        printf("%d\t", h->arr[i]);
    }
    printf("\n");

    destroy(h);
}

void test_case_2() {
    heap *h;
    int len = 5;
    int capacity = 6, i, data[5] = {3, 1, 6, 9, 5};
    h = heap_create(capacity);

    for (i = 0; i < len;i++) {
        h->arr[i + 1] = data[i];   
    }
    h->count = len;

    for (i = 1; i <= h->count; i++) {
        printf("%d\t", h->arr[i]);
    }
    printf("\n");


    buildHeap(h);
    printf("----- buildHeap ---- \n");
    for (i = 1; i <= h->count; i++) {
        printf("%d\t", h->arr[i]);
    }
    printf("\n");

    printf("----- sort ---- \n");
    sort(h);
    for (i = 1; i <= h->count; i++) {
        printf("%d\t", h->arr[i]);
    }
    printf("\n");
    destroy(h);
}

int main() {
    test_case_2();
    return 0;
}

