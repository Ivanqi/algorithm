#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define printState(states, n, w) \
    for (int i = 0; i < n; ++i) { \
        for (int j = 0; j < w + 1; ++j) { \
            printf("%d ", states[i][j]); \
        } \
        printf("\n"); \
    }

int minCoins(int money) {
    if (money == 1 || money == 3 || money == 5) return 1;

    int state[money][money + 1];
    int i, j;
    for (i = 0; i < money; i++) {
        for (j = 0; j < money + 1; j++) {
            state[i][j] = -1;
        }
    }

    if (money >= 1) state[0][1] = 1;
    if (money >= 3) state[0][3] = 3;
    if (money >= 5) state[0][5] = 5;

    int minNums = 0;

    for (i = 1; i < money; i++) {
        for (j = 1; j <= money; j++) {
            if (state[i - 1][j] != -1) {
                if (j + 1 <= money) state[i][j + 1] = 1;
                if (j + 3 <= money) state[i][j + 3] = 3;
                if (j + 5 <= money) state[i][j + 5] = 5;
            }
        }
        if (state[i][money] >= 0) { minNums = i + 1; break; };
    }

    // printState(state, money, money);

    for (i = minNums - 1; i >= 0; i--) {
        for (j = money; j > 0; j--) {
            if (state[i][j] > 0) {
                printf("硬币:%d\n", state[i][j]);
                j = j - state[i][j];
                break;
            }
        }
    }
    return minNums;
}

int main() {

    int money = 9;

    int ret = minCoins(money);
    printf("结果为:%d\n", ret);
}