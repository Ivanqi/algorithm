#include <stdio.h>
#include <stdlib.h>

void printState(int states[10], int w) {
    int j;
    for (j = 0; j < w + 1; ++j) {
        printf("%d ", states[j]);
    }
    printf("\n");
}

int knapsack3(int *weight, int *values, int n, int w) {
    int states[w + 1];
    int i, j, v;

    for (j = 0; j < w + 1; ++j) {
        states[j] = -1;
    }

    states[0] = 0;
    if (weight[0] <= w) {
        states[weight[0]] = values[0];
    }
    printState(states, w);
    for (i = 1; i < n; ++i) {                   // 动态规划，状态转移
        for (j = 0; j <= w - weight[i]; ++j) {  // 选择第i个物品
            if (states[j] >= 0) {
                v = states[j] + values[i];
                if (v > states[j + weight[i]]) {
                    states[j + weight[i]] = v;
                }
            }
        }
    }
    printState(states, w);
    // 找出最大值
    int maxvalue = -1;
    for (j = 0; j <= w; ++j) {
        if (states[j] > maxvalue) maxvalue = states[j];
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