#include <midnum.h>

MidNum* midnum_create() {
    MidNum* tmp = (MidNum*)malloc(sizeof(MidNum));
    assert(tmp != NULL);

    tmp->count = 0;
    tmp->MinHeap = NULL;
    tmp->MaxHeap = NULL;

    return tmp;
}

void midnum_insert(MidNum* mid, int data) {
    mid->count++;

    if (heap_empty(mid->MaxHeap) && heap_empty(mid->MinHeap)) {
        heap_insert(mid->MaxHeap, data);
        return ;
    }

    if (data <= heap_top(mid->MaxHeap)) {
        heap_insert(mid->MaxHeap, data);
    } else {
        heap_insert(mid->MinHeap, data);
    }

    int balanceVal = mid->count / 2;
    if (mid->MaxHeap->count > balanceVal) {
        midnum_move(mid->MaxHeap, mid->MinHeap, mid->MaxHeap->count - balanceVal);
        return;
    }

    if (mid->MinHeap->count > balanceVal) {
        midnum_move(mid->MinHeap, mid->MaxHeap, mid->MinHeap->count - balanceVal);
        return;
    }

}
int midnum_getmid(MidNum* mid) {
    return mid->MaxHeap->array[1];
}

void midnum_move(Heap *src, Heap *out, int move_num) {
    int i;
    for (i = 0; i < move_num; i++) {
        heap_insert(out, heap_pop(src));
    }
}


int main() {
    MidNum* mid = midnum_create();
    mid->MinHeap = heap_create(CAP_LEN, min_cmp);
    mid->MaxHeap = heap_create(CAP_LEN, max_cmp);

    int i;
    for (i = 1; i <= 100; i++) {
        midnum_insert(mid, i);
    }
    printf("--------- 大顶堆 ------------ \n");
    for (i = 1; i <= 50; i++) {
        printf("%d\t", mid->MaxHeap->array[i]);
    }
    printf("\n");
     printf("--------- 小顶堆 ------------ \n");
    for (i = 1; i <= 50; i++) {
        printf("%d\t", mid->MinHeap->array[i]);
    }
    printf("\n");
    printf("中值:%d\n", midnum_getmid(mid));
    return 0;
}