#include <stdio.h>
#include <stdlib.h>

int GetCellCount(int n) {
    if (n < 0) return 0;

    if (n == 0) return 1;

    if (n == 1) return 2;

    if (n == 2) return 4;

    if (n == 3) return 7;

    return 2 * GetCellCount(n - 1) - GetCellCount(n - 4);
}

int main() {

    int n = 4;
    int ret = GetCellCount(n);
    printf("结果为: %d\n", ret);
    return 0;
}