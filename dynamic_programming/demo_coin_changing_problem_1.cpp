#include <iostream>
#include <algorithm>

using namespace std;

static const int MMAX = 20;
static const int NMAX = 50000;
static const int INFTY = (1 << 29);

/***
 * 1. C[m] 一维数组，C[i]表示第i种硬币面值　
 * 2. T[m][n + 1] 二维数组，T[i][j]表示使用第0至第i种硬币支付j元时的最少硬币数
 * 3. T[i][j] 求解
 *   1. T[i][j] 只有“使用第i种硬币”和“不使用第i种硬币”两种情况
 *   2. 递归式： T[i][j] = min(T[i - 1][j], T[i][j - C[i]] + 1)
 *      1. T[i - 1][j] 为 不使用第i种硬币。 主要为了在j总额前，i - 1面额的值是多少。用 i 和 i - 1 面额进行对比
 *      2. T[i][j - C[i]] + 1。 为 使用第i种硬币. j - C[i] = x.当前金额 - 当前面额 = 之前金额的面额数量。且 x + 1(下一个金额数) = 当前金额
 * 4. 我们没必要给每一种面值都记录最有枚数因此支付j元时的最少枚数可以作为一维数组元素T[j]. 由下面的式子求得
 *      1. T[j] = min(T[j], T[j - C[i]] + 1);
 */

int main () {

    int n = 15, m = 6;
    int C[] = {1, 2, 7, 8, 12, 50};
    int T[NMAX + 1];
    
    for (int i = 0; i <= NMAX; i++) T[i] = INFTY;
    T[0] = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 0; j + C[i] <= n; j++) {
            T[j + C[i]] = min(T[j + C[i]], T[j] + 1);
        }
    }

    cout << T[n] << endl;
    return 0;
}