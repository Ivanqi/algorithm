#include <stdio.h>
#include <stdlib.h>

int* insertionSort(int A[], int N) {
    if (N <= 1) return A;
    int i, j, v;
    for (i = 1; i < N; ++i) {
        v = A[i];
        j = i - 1;
        for (; j >= 0; --j) {
            if (A[j] > v) {
                A[j + 1] = A[j];    // 数据移动
            } else {
                break;
            }
        }
        A[j + 1] = v;               // 插入数据
    }
    return A; 
}

/**
 * 我们将数组分成两个区间，已排序区间和未排序区间
 *  1. 初始已排序区间只有一个元素，就是数组插入的第一个元素
 *  2. 插入算法的核心思想就是取未排序区间中的元素，在已排序区间中找到合适的插入位置将其插入
 *  3. 并且保证已排序区间数据一直有序。重复这个过程，直到未排序区间中元素为空，算法结束
 */
int main() {

    int arr[] = {4, 5, 6, 1, 3, 2};
    int n = 6;

    int *res = insertionSort(arr, n);
    for (int i = 0; i < n; i++) {
       printf("val: %d \n", res[i]);
    }
    return 0;
}