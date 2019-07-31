/***
 * 斐波那契数列-动态规划
 * 动态转移方程
 *  1. n = 0, 1
 *  2. n = 1, 1
 *  3. fib(n + 1) + fib(n -1)
 */

#include<iostream>
using namespace std;

int main () {

    int n; cin >> n;
    int F[50];
    F[0] = F[1] = 1;

    for (int i = 2; i <= n; i++) F[i] = F[i - 1] + F[i - 2];

    cout << F[n] << endl;
    return 0;
}
