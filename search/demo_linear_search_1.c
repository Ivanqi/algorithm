#include <stdio.h>

// 线性搜索
int search(int A[], int n, int key) {
    int i = 0;
    A[n] = key;
    while (A[i] != key) i++;
    return i != n;
}

int main() {

    int i, n = 5 , A[10000 + 1], q, key, sum = 0;

    scanf("%d", &n);

    for (i = 0; i < n; i++) scanf("%d", &A[i]);

    scanf("%d", &q);

    for (i = 0; i < q; i++) {
        scanf("%d", &key);
        if (search(A, n, key)) sum++;
    }

    printf("%d\n", sum);
    return 0;
}