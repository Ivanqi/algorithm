#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int partition(char a[], int left, int right) {
    int pivot, i, j, t;
    pivot = a[left];
    i = left;
    j = right + 1;

    while (1) {
        do i++; while (a[i] <= pivot && i <= right);
        do j--; while (a[j] > pivot);
        if (i >= j) break;
        t = a[i]; a[i] = a[j]; a[j] = t;
    } 
    t = a[left];
    a[left] = a[j];
    a[j] = t;
    return j;
}

void quickSortChar(char a[], int left, int right) {
    int j;
    if (left < right) {
        j = partition(a, left, right);
        quickSortChar(a, left, j - 1);
        quickSortChar(a, j + 1, right);
    }
}

void main() {

    char arr[] = "bacgd";
    puts(arr);
    quickSortChar(arr, 0, strlen(arr) - 1);
    puts(arr);
}