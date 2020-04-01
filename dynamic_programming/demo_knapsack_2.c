#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 时间复杂度O(n)， 空间复杂度O(w)
int knapsack2(int *items, int n, int w)
{
    bool states[w + 1];
    states[0] = true;
    if (items[0] <= w) {
        states[items[0]] = true;
    }
    int i, j;
    for (i = 1; i < n; ++i) {                   // 动态规划
        printf("note1:w:%d, i:%d, items[i]:%d\n", w, i, items[i]);
        for (j = w - items[i]; j >= 0; --j) {   // 把第i个物品放入背包
        // for (j = 0; j <= w - items[i]; ++j) {
            printf("j:%d\n", j);
            if (states[j] == true) states[j + items[i]] = true;
        }
    }

    for (i = w; i >= 0; --i) {  // 输出结果
        if (states[i] == true) return i;
    }

    return 0;
}

int main() {
    int weight[] = {2, 2, 4, 6, 3};     // 物品重量
    int n = 5;                          // 物品个数
    int w = 9;                          // 背包承受的最大重量

    int ret = knapsack2(weight, n, w);
    printf("结果展示:%d\n", ret);
}