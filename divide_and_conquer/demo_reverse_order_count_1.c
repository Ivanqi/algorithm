#include <stdio.h>
#include <stdlib.h>

int num = 0;

void merge(int *a, int p, int q, int r) {
    int i = p, j = q + 1, k = 0;
    int *tmp = (int *)malloc(sizeof(int) * (r - q + 1));
    // printf("p:%d, q:%d, r:%d, i:%d, j:%d, res: %d\n", p, q, r, i, j, i <= q && j <= r);
    while (i <= q && j <= r) {
        // printf("a[i]:%d, a[j]:%d\n", a[i], a[j]);
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            num += (q - i + 1);     // 统计p - q 之间，比a[j]大的元素个数
            // printf("q:%d, i:%d, num:%d\n", q, i, num);
            tmp[k++] = a[j++];
        }
    }
    // int m;
    // int length;
    // length = sizeof(tmp)/sizeof(tmp[0]);
    // for (m = 0; m < length; m++) {
    //     printf("x1-%d \t", tmp[m]);
    // }
    // printf("\n");
    while (i <= q) {    // 处理剩下的
        tmp[k++] = a[i++];
    }

    while (j <= r) {    // 处理剩下的
        tmp[k++] = a[j++];
    }

    // for (m = 0; m < length; m++) {
    //     printf("x2-%d \t", tmp[m]);
    // }
    // printf("\n");

    for (i = 0; i <= r - p; ++i) {  // 从tmp拷贝回a
        a[p + i] = tmp[i];
    }

    free(tmp);
}

void mergeSortCounting(int *a, int p, int r) {
    if (p >= r) return;
    int q = (p + r) / 2;
    mergeSortCounting(a, p, q);
    mergeSortCounting(a, q + 1, r);
    merge(a, p, q, r);
}

int count(int *a, int n) {
    num = 0;
    mergeSortCounting(a, 0, n - 1);
    return num;
}


int main() {
    int arr[] = {1, 6, 5, 2, 4, 3};
    int ret = count(arr, 6);
    printf("逆序对个数:%d\n", ret);
    int i;
    for (i = 0; i < 6; i++) {
        printf("%d \t", arr[i]);
    }
    printf("\n");
    return 0;
}