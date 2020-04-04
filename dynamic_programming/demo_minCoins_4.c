#include <stdio.h>
#include <stdlib.h>

#define printState(states, w) \
    for (int j = 0; j < w + 1; ++j) { \
        printf("%d ", states[j]); \
    } \
    printf("\n"); \


int minCoins(int money) {
    int states[money + 1];
    int i;

    for (i = 0; i < money + 1; i++) {
        states[i] = 0;
    }
    states[1] = 1;
    int m1, m2, m3 = money;

    for (i = 2; i < money + 1; i++) {
        if (i - 1 > 0) {
            m1 = states[i - 1];
        }

        if (i - 3 > 0) {
            m2 = states[i - 3];
        }

        if (i - 5 > 0) {
            m3 = states[i - 5];
        }
        if (m1 == money && m2 == money && m3 == money) break;
        // f(n) = 1 + min(f(n - 1), f(n - 3), f(n - 5))
        states[i] = 1 + (m1 < m3 ? (m1 < m2 ? m1 : m2) : (m3 < m2 ? m3 : m2));
    }

    // printState(states, money);
    return states[money];
}

int main () {
    int ret = minCoins(9);
    printf("结果为：%d\n", ret);
}