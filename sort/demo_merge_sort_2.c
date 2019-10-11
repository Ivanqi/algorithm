#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<assert.h>

#define SORT_MAX 1000000

void dump(int arr[], int size) {
    int idx;

    for (idx = 0; idx < size - 1; idx++) {
        printf("%d\n", arr[idx]);
    }
}

void __merge_sentry(int a[], int middle, int left, int right) {
    int *pleft = NULL;
    int *pright = NULL;
    int i = 0;
    int j = 0;
    int k = 0;
    int left_size = middle - left + 1;
    int right_size = right - middle;

    pleft = (int *)malloc(sizeof(int) * (left_size + 1));
    assert(pleft != NULL);
    pright = (int *) malloc(sizeof(int) * (right_size + 1));
    assert(pright != NULL);

    for (i = 0; i < left_size; i++) {
        pleft[i] = a[left + i];
    }
    pleft[left_size] = SORT_MAX;

    for (i = 0; i < right_size; i++) {
        pright[i] = a[middle + 1 +i];
    }

    for (k = left, i = 0, j = 0; k <= right; k++) {
        if (pleft[i] <= pright[j]) {
            a[k] = pleft[i++];
        } else {
            a[k] = pright[j++];
        }
    }

    free(pleft);
    free(pright);
    return;
}

void __merge_sort(int arr[], int left, int right) {

    int middle = 0;

	if(left >= right) {
		return;
	}

    if (left + 1 < right) {
        middle = (left + right) / 2;
        __merge_sort(arr, left, middle);
        __merge_sort(arr, middle + 1, right);
        __merge_sentry(arr, middle, left, right);
    }
}

void merge_sort(int arr[], int size) {
    __merge_sort(arr, 0, size - 1);
}

void merge_sort_test() {
    int test[10] = {5, 8, 9, 23, 67, 1, 3, 7, 31, 56};
    merge_sort(test, 10);
    dump(test, 10);
}

int main() {
    merge_sort_test();
    return 0;
}