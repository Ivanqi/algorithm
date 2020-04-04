#include <stdio.h>
#include <stdlib.h>

#define N 9
int men[N] = {0};

int minCoins(int money) {
    if (men[money] > 0) return men[money];
    if (money == 1 || money == 3 || money == 5) return 1;

    int m1, m2, m3 = money;
    if (money - 1 > 0) {
        m1 = minCoins(money - 1);
    }

    if (money - 3 > 0) {
        m2 = minCoins(money - 3);
    }
    if (money - 5 > 0) {
        m3 = minCoins(money - 5);
    }
    // f(n) = 1 + min(f(n - 1), f(n - 3), f(n - 5))
    int coins = 1 + (m1 < m3 ? (m1 < m2 ? m1 : m2) : (m3 < m2 ? m3 : m2));
    men[money] = coins;
    return coins;

}

int main () {
    int ret = minCoins(9);
    printf("结果为：%d\n", ret);
}