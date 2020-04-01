#include <stdio.h>
#include <stdlib.h>

void printState(int states[5][10], int n, int w) {
    int i, j;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < w + 1; ++j) {
            printf("%d ", states[i][j]);
        }
        printf("\n");
    }
}

int knapsack3(int *weight, int *values, int n, int w) {
    int states[n][w + 1];
    int i, j, v;

    for (i = 0; i < n; ++i) {   // 初始化states
        for (j = 0; j < w + 1; ++j) {
            states[i][j] = -1;
        }
    }

    states[0][0] = 0;
    if (weight[0] <= w) {
        states[0][weight[0]] = values[0];
    }

    printState(states, n, w);

    for (i = 1; i < n; ++i) {           // 动态规划，状态转移
        for (j = 1; j <= w; ++j) {      // 不选择第i个物品
            if (states[i - 1][j] >= 0) states[i][j] = states[i - 1][j];
        }

        for (j = 0; j <= w - weight[i]; ++j) {  // 选择第i个物品
            if (states[i - 1][j] >= 0) {
                v = states[i - 1][j] + values[i];
                if (v > states[i][j + weight[i]]) {
                    states[i][j + weight[i]] = v;
                }
            }
        }
    }
    printState(states, n, w);
    // 找出最大值
    int maxvalue = -1;
    for (j = 0; j <= w; ++j) {
        if (states[n - 1][j] > maxvalue) maxvalue = states[n - 1][j];
    }

    return maxvalue;
}

int main() {
    int weight[] = {2, 2, 4, 6, 3};     // 物品重量
    int values[] = {3, 4, 8, 9, 6};     // 物品价值
    int n = 5;                          // 物品个数
    int w = 9;                          // 背包承受的最大重量

    int ret = knapsack3(weight, values, n, w);
    printf("结果展示:%d\n", ret);
}