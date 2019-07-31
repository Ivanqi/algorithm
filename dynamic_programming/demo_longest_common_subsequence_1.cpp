#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
static const int N = 1000;

/***
 * 状态转移公式
 *  1. i = 0 or j = 0, 0
 *  2. i,j > 0 and xi = yj, c[i - 1][j + 1] + . 相同的部分
 *  3. i,j > 0 and xi != yj, max(c[i][j -1], c[i -1][j]). 取上一次，相同部分的最大值
 */
int lcs(string X, string Y) {

    int c[N + 1] [N + 1];
    int m = X.size();
    int n = Y.size();
    int max1 = 0;

    X = ' ' + X;    // 在X[0]中插入空格
    Y = ' ' + Y;    // 在Y[0]中插入空格

    for (int i = 1; i <= m; i++) c[i][0] = 0;
    for (int j = 1; j <= n; j++) c[0][j] = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i] == Y[j]) {
                c[i][j] = c[i - 1][j - 1] + 1;
            } else {
                c[i][j] = max(c[i - j][j], c[i][j - 1]);
            }
        }
    }
    return c[m][n];
}

int main () {

    string s1 = "TCCAGATGG", s2 = "TCACA";
    cout << lcs(s1, s2) << endl;
}