#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * weight: 物品重量
 * n: 物品个数
 * w: 背包可承载重量
 */
// 时间复杂度O(n * w)， 空间复杂度O(n * w)
int knapsack(int *weight, int n, int w) {
    bool states[n][w + 1];    // 默认值false 
    states[0][0] = true;                // 第一行的数据要特殊处理，可以利用哨兵优化

    if (weight[0] <= w) {
        states[0][weight[0]] = true;
    }
    int i, j;
    for (i = 1; i < n; ++i) {       // 动态规划状态转移
        for (j = 0; j <= w; ++j) {  // 不把第i个物品放入背包
            // printf("node1|i:%d, i-1:%d, j:%d\n", i, i - 1, j);
            if (states[i - 1][j] == true) states[i][j] = states[i - 1][j];
        }
        // printf("w:%d, i:%d, weight[i]:%d, wwegiht:%d\n", w, i, weight[i], w - weight[i]);
        for (j = 0; j <= w - weight[i]; ++j) {  // 把第i个物品放入背包
            // printf("node2|i-1:%d, j:%d\n", i - 1, j);
            if (states[i - 1][j] == true) states[i][j + weight[i]] = true;
        }
    }

    for (i = w; i >= 0; --i) {  // 输出结果
        if (states[n - 1][i] == true) return i;
    }

    return 0;
}

int main() {
    int weight[] = {2, 2, 4, 6, 3};     // 物品重量
    int n = 5;                          // 物品个数
    int w = 9;                          // 背包承受的最大重量

    int ret = knapsack(weight, n, w);
    printf("结果展示:%d\n", ret);
}