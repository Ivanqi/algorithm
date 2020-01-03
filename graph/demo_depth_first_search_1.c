#include <stdio.h>
#define N 100
#define WHITE 0
#define GRAY 1
#define BLACK 2

int n, M[N][N];
int color[N], d[N], f[N], tt;

// 用递归函数实现的深度优先搜索
void dfs_visit(int u) {
    int v;
    color[u] = GRAY;
    d[u] = ++tt;     // 最初的访问
    for (v = 0; v < n; v++) {
        if (M[u][v] == 0) continue;
        if (color[v] == WHITE) {
            dfs_visit(v);
        }
    }
    color[u] = BLACK;
    f[u] = ++tt;    // 访问结束
}

void dfs() {
    int u;
    // 初始化
    for (u = 0; u < n; u++) color[u] = WHITE;
    tt = 0;

    for (u = 0; u < n; u++) {
        // 以未访问的u为起点进行深度优先搜索
        if (color[u] == WHITE) dfs_visit(u);
    }

    for (u = 0; u < n; u++) {
        printf("%d %d %d\n", u + 1, d[u], f[u]);
    }
}

/**
    6
    1 2 2 3
    2 2 3 4
    3 1 5
    4 1 6
    5 1 6
    6 0
 */
int main() {

    int u, v, k, i, j;
    scanf("%d", &n);

    for (i = 0; j < n; i++) {
        for (j = 0; j < n; j++) M[i][j] = 0;
    }

    for (i = 0; i < n; i++) {
        scanf("%d %d", &u, &k);
        u--;
        for (j = 0; j < k; j++) {
            scanf("%d", &v);
            v--;
            M[u][v] = 1;
        }
    }

    dfs();

    return 0;
}