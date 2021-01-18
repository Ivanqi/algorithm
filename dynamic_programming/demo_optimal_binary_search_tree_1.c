/**
 * 最优二叉查找树
 * 参考资料：https://www.cnblogs.com/fstang/archive/2013/01/06/2848339.html
 */
#include <stdio.h>
#include <float.h>
#define N 7
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
double dp[N + 1][N + 1];
double w_acc[N + 1][N + 1]; // w_acc[i][j] = w[i] + w[i + 1] + ... + w[j]

// compute w_acc 即连续若干node的频率之和
void preprocess() {
    double w[] = {0, .005, .4, .08, .1, .1, .23};
    for (int i = 1; i <= N; i++) {
        w_acc[i][i - 1] = 0;
        for (int j = i; j <= N; j++) {
            w_acc[i][j] = w_acc[i][j - 1] + w[j];
        }
    }
}

void obst() {
    for (int i = 1; i <= N; i++) {
        dp[i][i - 1] = 0;
    }

    for (int k = 0; k <= N - 1;k++) {   // k = j - 1, node[i..j]
        for (int i = 1; i <= N - k; i++) {
            int j = i + k;
            dp[i][j] = DBL_MAX;
            for (int r = i; r <= j; r++) {  // possible positions of root
                dp[i][j] = min(w_acc[i][j] + dp[i][r - 1] + dp[r + 1][j], dp[i][j]);
            }
        }
    }
}

int main() {

    preprocess();
    obst();
    printf("%lf\n", dp[1][N]);
    return 0;
}