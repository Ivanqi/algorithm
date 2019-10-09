#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

struct barrel {   
    int node[10];   
    int count;/* the num of node */  
};

void dump(int a[], int size) {
    int i = 0;
    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

int partition(int a[], int left, int right) {
    int i = left;
    int j = right;
    int key = a[left];

    while (i < j) {
        while ((i < j) && (a[j] > key)) {
            j--;
        }

        if (i < j) {
            a[i] = a[j];
        }

        while ((i < j) && a[i] <= key) {
            i++;
        }

        if (i < j) {
            a[j] = a[i];
        }
    }
}

void quick_sort(int a[], int left, int right) {
    int q = 0;
    // 递归终止条件
    if (left >= right) {
        return;
    }

    q = partition(a, left, right);
    quick_sort(a, left, (q - 1));
    quick_sort(a, (q + 1), right);
}

void bucket_sort(int data[], int size) {
    int max, min, num, pos;
    int i, j, k;
    struct barrel *pBarrel;

    max = min = data[0];
    for (i = 1; i < size; i++) {
        if (data[i] > max) {
            max = data[i];
        } else if (data[i] < min) {
            min = data[i];
        }
    }

    num = (max - min + 1) / 10 + 1;
    pBarrel = (struct barrel *)malloc(sizeof(struct barrel) * num);
    memset(pBarrel, 0, sizeof(struct barrel) * num);

    for (i = 0; i < size; i++) {
        k = (data[i] - min + 1) / 10;
        (pBarrel + k)->node[(pBarrel + k)->count] = data[i];
        (pBarrel + k)->count++;
    }

    pos = 0;
    for (i = 0; i < num; i++) {
       if ((pBarrel + i)->count != 0) {
           quick_sort((pBarrel + i)->node, 0, ((pBarrel + i)->count) - 1);

           for (j = 0; j < (pBarrel + i)->count; j++) {
               data[pos++] = (pBarrel + i)->node[j];
           }
       } 
    }
    free(pBarrel);
}

void bucket_sort_test() {
    int a[] = {78, 17, 39, 26, 72, 94, 21, 12, 23, 91};
    int size = sizeof(a) / sizeof(int);
    printf("\n bucket sort test ...");
    bucket_sort(a, size);
    dump(a,size);
}

int main() {
    bucket_sort_test();
    return 0;
}