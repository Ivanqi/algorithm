#include <stdio.h>
#include <stdlib.h>

/**
 * 伯努利实验，实验n次，出现k次的概率
 * 非递归方式实现，采用二维数组存储已经计算过的值
 * 时间复杂度: O(n * k) 空间复杂度: O(n * k)
 * 
 * @param n
 * @param k
 * @param p
 * @return
 */

double binomialArray(int n, int k, double p) {
    double array[n + 1][k + 1];

    // 初始化数组
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= k; j++) {
            array[i][j] = 0;
        }
    }

    array[0][0] = 1.0;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= k; j++) {
            if (j > i) {
                break;
            }

            if (i - 1 >= 0) {
                array[i][j] += (1.0 - p) * array[i - 1][j];
                if (j - 1 >= 0) {
                    array[i][j] += p * array[i - 1][j - 1];
                }
            }
        }
    }

    return array[n][k];
}

int main() {

    int N, K = 5;
    double p = 0.5;

    double ret = binomialArray(N, K, p);
    printf("结果为:%d\n", ret);
    return 0;
}