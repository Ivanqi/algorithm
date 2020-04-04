#include <stdio.h>
#include <stdlib.h>

int maxCount = 99999;

int minCoins(int money, int cw, int num) {
    // printf("money:%d, cw%d, num:%d\n", money, cw, num);
    if (cw == money) {
        if (maxCount > num) maxCount = num;
        return ;
    }

    if (cw < money) {
        minCoins(money, cw + 1, num + 1);
        minCoins(money, cw + 3, num + 1);
        minCoins(money, cw + 5, num + 1);
    } else {
        return;
    }
}


int main() {
    minCoins(9, 0, 0);
    printf("结果为:%d\n", maxCount);
}