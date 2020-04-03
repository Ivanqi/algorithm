#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define N 4
#define MAX_NUM 99999
int mem[N][N]{{0}};
int n = N;

vector<vector<int>> matrix = {
    {1, 3, 5, 9},
    {2, 1, 3, 4},
    {5, 2, 6, 7},
    {6, 8, 4, 3}
};

int minDist(int i, int j) { // 调用 minDist(n - 1, n - 1)

    if (i == 0 && j == 0) return matrix[i][j];

    if (mem[i][j] > 0) return mem[i][j];

    int minLeft = MAX_NUM;
    if (j - 1 >= 0) {
        minLeft = minDist(i, j - 1);
    }

    int minUp = MAX_NUM;
    if (i - 1 >= 0) {
        minUp = minDist(i - 1, j);
    }

    int currMinDist = min(minLeft, minUp) + matrix[i][j];
    mem[i][j] = currMinDist;
    return currMinDist;
}

int main () {

    int ret = minDist(N - 1, N - 1);
    cout << "结果为:" << ret << endl;
}