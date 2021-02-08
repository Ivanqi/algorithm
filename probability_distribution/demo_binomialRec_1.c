#include <stdio.h>
#include <stdlib.h>

/**
 * 时间复杂度: O(2 ^ n)
 * @param N
 * @param K
 * @param p
 * @return 伯努实验n次，事件发生k次概率
 * 
 */
double binomialRec(int N, int K, double p) {
    if (N == 0 && K == 0) {
        return 1.0;
    }

    if (N < 0 || K < 0) {
        return 0.0;
    }

    return (1.0 - p) * binomialRec(N - 1, K, p) + p * binomialRec(N - 1, K - 1, p);
}

int main() {

    int N, K = 5;
    double p = 0.5;

    double ret = binomialRec(N, K, p);
    printf("结果为:%d\n", ret);
    return 0;
}
