#include <iostream>
#include <stdio.h>
// 参考资料：https://blog.csdn.net/a447332241/article/details/87819537
using namespace std;
int c[100][100];    // 邻接矩阵
int color[100];     // 记录每个顶点的颜色
int count, m, n;    // count记录方案数 n个顶点 m种颜色

int check(int k) {  // 检查第i个顶点的颜色是否满足条件
    for (int i = 1; i <= k; i++) {
        if (c[k][i] == 1 && color[i] == color[k]) { // k 与 i之间相连并且i顶点的颜色与k顶点的颜色相同
            return 0;
        }
    }
    return 1;
}

void graphColor(int step) {
    // printf("step:%d, color[step]:%d\n", step, color[step]);
    if (step == n + 1) {    // 表示前面所有的顶点颜色都已经填完
        for (int i = 1; i <= n; i++) {
            printf("%d ", color[i]);
        }
        count++;
        printf("\n\n");
        return;
    } else {
        for (int i = 1; i <= m; i++) {
            color[step] = i;        // 首先将这个顶点颜色换为i
            if (check(step) == 1) { // 检查是否符合条件
                graphColor(step + 1);
            }
            color[step] = 0;        // 回溯 置为0
        }
    }
}

/*
input
5 4 
0 1 1 1 0 
1 0 1 1 1 
1 1 0 1 0 
1 1 1 0 1 
0 1 0 1 0

output 
48

 */
int main() {

    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> c[i][j];
        }
    }
    printf("\n\n");
    graphColor(1);
    printf("%d", count);
    return 0;
}