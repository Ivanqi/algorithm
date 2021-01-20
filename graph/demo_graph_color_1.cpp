#include <stdio.h>
#include <iostream>

#define V 4 // 图中的顶点数

// 打印解决方案的实用函数
void printSolution(int color[]) {
    printf("Following are the assigned colors \n");
    for (int i = 0; i < V; i++) {
        printf(" %d", color[i]);
    }
    printf("\n\n");
}

bool isSafe(int v, bool graph[V][V], int color[], int c) {  // 用于检查当前颜色分配的实用程序
    for (int i = 0; i < v; i++) {
        if (graph[v][i] && c == color[i]) {
            return false;
        }
    }

    return true;
}

void graphColoring(bool graph[V][V], int m, int color[], int v) {   // 求解m着色问题的递推效用函数
    if (v == V) {   // 基本情况： 如果所有顶点都指定了颜色，则返回真
        printSolution(color);
        return ;
    }

    // 考虑这个顶点v并尝试不同的颜色
    for (int c = 1; c <= m; c++) {
        // 检查颜色C到V的分配是否正确
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            printf("color[%d]: %d\n", v, c);
            // 递归为其余顶点指定颜色
            graphColoring(graph, m, color, v + 1);
            // 如果指定颜色C不会导致解决方案然后删除它
            color[v] = 0;
        }
    }
}

int main () {

    /* Create following graph and test whether it is 3 colorable
    (3)---(2)
    |   / |
    |  /  |
    | /   |
    (0)---(1)
    */

    bool graph[V][V] = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 1, 0}
    };
    int m = 3;  // Number of colors

    int color[V];
    
    for (int i = 0; i < V; i++) {
        color[i] = 0;
    }

    graphColoring(graph, m, color, 0);
    return 0;
}