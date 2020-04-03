#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

int minDist = 9999;

void minDistBT(int i, int j, int dist, vector<vector<int>>& w, int n) {
    // printf("i:%d, j:%d, dist:%d, minDist:%d\n", i, j, dist, minDist);
    if (i == n && j == n) {
        if (dist < minDist) minDist = dist;
        return;
    }

    if (i < n) {   
        minDistBT(i + 1, j, dist + w[i][j], w, n);  // 往下走，更新i = i + 1, j = j
       
    }

    if (j < n) {
        minDistBT(i, j + 1, dist + w[i][j], w, n);  // 往右走，更新i = i, j = j + 1
    }
}

int main() {
    vector<vector<int>> matrix = {
        {1, 3, 5, 9},
        {2, 1, 3, 4},
        {5, 2, 6, 7},
        {6, 8, 4, 3}
    };

    int n = 4;

    minDistBT(0, 0, 0, matrix, n - 1);

    cout << "结果为:" << minDist << endl;
}