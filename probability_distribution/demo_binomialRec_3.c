#include <stdio.h>
#include <stdlib.h>

/**
 * 伯努利实验，实验n次，出现k次的概率
 * 非递归方式实现，采用一维数组实现存储计算过的值
 * 时间复杂度: O(n * k), 空间复杂度: O(k)
 */
double binomial(int n, int k, double p) {
    double array[k + 1];
    for (int i = 0; i < k + 1; i++) {
        array[i] = 0.0;
    }

    array[0] = 1.0;
    for (int i = 1; i <= n; i++) {
        // 这里要倒着计算，因为正序计算新值会覆盖掉之间的旧值
        for (int j = k; j >= 0; j--) {
            if (j - 1 >= 0) {
                array[j] = (1.0 - p) * array[j] + p * array[j - 1];
            } else {
                array[j] = (1.0 - p) * array[j]; 
            }
        }
    }

    return array[k];
}

int main() {

    int N, K = 5;
    double p = 0.5;

    double ret = binomial(N, K, p);
    printf("结果为:%d\n", ret);
    return 0;
}