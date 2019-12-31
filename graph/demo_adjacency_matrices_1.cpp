#include <iostream>
using namespace std;
static const int N = 100;

/*
4
1 2 3 4
2 1 4
3 0
4 1 3
 */
int main() {

    int M[N][N];    // 0 0 起点的领接矩阵
    int n, u, k, v;

    cin >> n;

    for (int i = 0; i < n; i++ ) {
        for (int j = 0; j < n; j++) M[i][j] = 0;
    }

    for (int i = 0; i < n; i++) {
        cin >> u >> k;
        u--;    // 转换为0起点
        for (int j = 0; j < k; j++) {
            cin >> v;
            v--;    // 转换为0起点
            M[u][v] = 1; // 在u 和 v之间画出一条边
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j) cout << " ";
            cout << M[i][j];
        }
        cout << endl;
    }

    return 0;
}