#include <stdio.h>
#include <stdlib.h>

int A[1000000];

/**
 * 将 A[i] 插入到所在分组的正确位置上
 * A[i] 所在分组为
 *  A[i - 2 * gap], A[i - gap], A[i], A[i + gap], A[i + 2 * gap]
 */
void insertionSort(int gap, int i) {
    int inserted = A[i];
    int j;
    
    // 插入的时候按组进行插入(组内元素两两相隔gap)
    for (j = i - gap; j >= 0 && inserted < A[j]; j -= gap) {
        A[j + gap] = A[j];
    }
    A[j + gap] = inserted;
}

void shellSort(int N) {
    if (N <= 1) return;
    int gap, i;

    // 进行分组，最开始时的增量(gap)为数组的一半
    for (gap = N / 2; gap > 0; gap /= 2) {
        // 对各个分组进行插入排序
        for (i = gap; i < N; i++) {
            insertionSort(gap, i);
        }
    }
}

int main() {
    int temp[] = {5, 7, 8, 3, 1, 2, 4, 6};
    int n = 8;
    int i;

    for (i = 0; i < n; i++) {
        A[i] = temp[i];
    }

    shellSort(n);

    for (i = 0; i < n; i++) {
        printf("val: %d\n", A[i]);
    }
    return 0;
}