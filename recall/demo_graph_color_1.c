#include <stdio.h>
// 参考资料: https://blog.csdn.net/qq_21149391/article/details/79507153
#define N 5
int map[N][N] = {
    0, 1, 1, 0, 1,
    1, 0, 1, 0, 1,
    1, 1, 0, 1, 0,
    0, 0, 1, 0, 1,
    1, 1, 0, 1, 0
};  // 五个点，七条边的图，邻接矩阵表示

int nums = 0;

/**
 * 用四种颜色给地图着色，要求相邻颜色不同，图用连接矩阵存储，求所有着色方案
 * 给第n个点着色，col=1表示已经着色，col=0表示未着色，sign=0表示未验证，sign=1表示已验证且可行
 * 有以下三种情况:  col     sign
 * 0        0   //未着色，则着色
 * 1        0   //已着色但未验证
 * 1        1   //着色且此色可用
 * 当 n = N - 1，即最后一位，若此时col == 1, sign == 1，则可打印输出
 * 验证的作用是什么？
 *  1. 为了找到相邻图着色不冲突的方案
 * 相邻怎么定义？
 *  1. 有连接意味着相邻
 */

void inputColor(int n, int *color, int col, int sign) {
    int i;
    for (i = n + 1; i < N; i++) {       // 消除之前着色的痕迹
        color[i] = 0;
    }
    // printf("n:%d, col:%d, color[n]:%d, sign:%d\n", n, col, color[n], sign);
    if (col == 0) {                     // 未着色，则着色
        for (i = 1; i <= 4; i++) {
            color[n] = i;               // 颜色 1 - 4
            inputColor(n, color, 1, 0);
        }
    } else if (col == 1 && sign == 0) { // 已着色但未验证，则验证此位
        for (i = 0; i < N; i++) {
            if (map[n][i] == 1 && color[n] == color[i]) return;
        }
        inputColor(n, color, 1, 1);     // 此色可用，sign设置为1
    } else if (col == 1 && sign == 1) { // 着色且此色可用
        if (n < N - 1) {                // 进行下一位
            inputColor(n + 1, color, 0, 0);
        } else if (n == N - 1) {        // 若此位是最后一位，则打印输出
            nums++;
            for (i = 0; i < N; i++) {
                printf("%d\t", color[i]);
            }
            printf("\n");
        }
    }
}

int main() {

    int color[5] = {0};
    inputColor(0, color, 0, 0);
    printf("总有有%d种方案\n", nums);
    return 0;
}