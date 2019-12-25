#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <priorityQueue.h>

#define CAP_LEN 50

typedef struct midnum {
    int count;
    Heap* MinHeap;
    Heap* MaxHeap;
} MidNum;


int min_cmp(int a, int b) {
    if (a < b) return 1;
    else return 0;
}

int max_cmp(int a, int b) {
    if (a > b) return 1;
    else return 0;
}



MidNum* midnum_create();
void midnum_insert(MidNum* mid, int data);
int midnum_getmid(MidNum* mid);
void midnum_move(Heap *src, Heap *out, int move_num);

