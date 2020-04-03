#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define printState(states, n, w) \
    for (int i = 0; i < n; ++i) { \
        for (int j = 0; j < w; ++j) { \
            printf("%d ", states[i][j]); \
        } \
        printf("\n"); \
    }

#define N 4
#define MAX_NUM 99999

/**
 * 动态转移方程为: states[i][j] = min(states[i - 1][j], states[i - 1][j - 1]) + matrix[i][j];
 */
int minDistDP(vector<vector<int>>& matrix, int n) {
    int states[N][N] = {{0}};
    int i, j;

    int sum = 0;
    for (i = 0; i < n; ++i) {   // 初始化states的第一列数据
        sum += matrix[i][0];
        states[i][0] = sum;
    }

    sum = 0;
    for (j = 0; j < n; ++j) {   // 初始化states的第一行数据
        sum += matrix[0][j];
        states[0][j] = sum;
    }

    for (i = 1; i < n; i++) {
        for (j = 1; j < n; j++) {
            states[i][j] = min(states[i - 1][j], states[i][j - 1]) + matrix[i][j];
        }
    }

    printState(states, n, n);
    return states[n - 1][n -1];
}

int main() {

    vector<vector<int>> matrix = {
        {1, 3, 5, 9},
        {2, 1, 3, 4},
        {5, 2, 6, 7},
        {6, 8, 4, 3}
    };

    int n = N;

    int ret = minDistDP(matrix, n);

    cout << "结果为:" << ret << endl;
}