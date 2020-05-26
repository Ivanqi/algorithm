#include <stdio.h>
#include <stdlib.h>

/**
 * 1个细胞的生命周期是3小时，1小时分裂一次。求n个小时后，容器内有多少细胞？同时分析时间复杂度
 *  细胞分裂（cell division）是指活细胞增殖其数量由一个细胞分裂为两个细胞的过程
 *  分裂前的细胞称母细胞，分裂后形成的新细胞称子细胞
 *  通常包括细胞核分裂和细胞质分裂两步。在核分裂过程中母细胞把遗传物质传给子细胞
 *  分裂一次得到2个细胞
 */
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