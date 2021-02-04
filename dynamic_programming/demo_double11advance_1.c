#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define printState(states, n, w) \
    for (int i = 0; i < n; ++i) { \
        for (int j = 0; j < w; ++j) { \
            printf("%d ", states[i][j]); \
        } \
        printf("\n"); \
    }   

// items 商品价格，n商品个数，w表示减满条件，比如200
void double11advance(int *items, int n, int w) {
    int len = 3 * w + 1;
    bool states[n][len];
    int i, j;

    for (i = 0; i < n; ++i) {                  // 初始化states
        for (j = 0; j < w + 1; ++j) {
            states[i][j] = false;
        }
    }

    states[0][0] = true;                       // 第一行的数据要特殊处理
    if (items[0] < len - 1) {
        states[0][items[0]] = true;
    }
    // printState(states, n, len);
    // printf("\n");
    for (i = 1; i < n; i++) {
        for (j = 0; j <= len; ++j) {            // 不购买第i个商品
            if (states[i - 1][j] >= 0) states[i][j] = states[i - 1][j];
        }
        for (j = 0; j < (len - 1) - items[i]; ++j) {    // 购买第i个商品
            if (states[i - 1][j] == true) states[i][j + items[i]] = true;
        }
    }
    // printState(states, n, len);
    for (j = w; j < len; ++j) {
        if (states[n - 1][j] == true) break;    // 输出结果大于等于w的最小值
    }
    if (j == len) return;                       // 没有可行解

    for (i = n - 1; i >= 1; --i) {              // i 表示二维数组中的行，j表示列
        // printf("i:%d, j:%d, j - items[i]:%d, states:%d\n", i, j, items[i], states[i - 1][j - items[i]]);
        if (j - items[i] >= 0 && states[i - 1][j - items[i]] == true) {
            printf("buys:%d ", items[i]);            // 购买这个商品
            j = j - items[i];
        } // else 没有购买这个商品，j不变
    }
    printf("\n");
    if (j != 0) printf("end:%d\n", items[0]);
}

int main() {
    int items[] = {10, 30, 29, 45, 67, 100, 34, 88, 90, 78};
    int n = 10;
    int w = 200;

    double11advance(items, n, w);
}