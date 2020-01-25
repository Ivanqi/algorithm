#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct barrel {   
    int node[10];   
    int count;      /* the num of node */  
};

void dump(int a[], int size) {
    int i = 0;
    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

void swap (int *a , int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int parition(int *arr, int p, int r) {
    int i, j;
    i = j = p;

    for (; j < r; j++) {
        if (arr[j] < arr[r]) {
            if (i != j) {
                swap(arr + i, arr + j);
            }
            i++;
        }
    }
    swap(arr + i, arr + j);
    return i;
}

void quickSort(int *arr, int left, int right) {
    if (left >= right) return ;

    int q = 0;
    q = parition(arr, left, right);
    quickSort(arr, left, q - 1);
    quickSort(arr, q + 1, right);
}

void bucket_sort(int data[], int size) {
    struct barrel *pBarrel;
    int max, min, num, i, j, pos, k;

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
            quickSort((pBarrel + i)->node, 0, ((pBarrel + i)->count) - 1);
        }

        for (j = 0; j < (pBarrel + i)->count; j++) {
            data[pos++] = (pBarrel + i)->node[j];
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