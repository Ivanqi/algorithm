/**
 * c++ DFS 生成 1 ～ n 的排列（不可重）
 * https://blog.csdn.net/u010651394/article/details/86542654
 */

#include <iostream>
using namespace std;
const int maxn = 1000;
int n;
int ans[maxn],vis[maxn];

void dfs (int dep) {

    if (dep == n + 1) {
        for (int i = 1; i <= n; i++) {
            cout << ans[i] << " ";
        }
        cout << endl;
        return;
    }

    for (int i = 1; i <= n; i++) {
        if (vis[i] == 0) {
            vis[i] = i;
            ans[dep] = i;
            dfs(dep + 1);
            vis[i] = 0;
        }
    }
}

int main () {
    while(cin >> n && n > 0) {
        fill_n(vis, n + 1, 0);
        dfs(1);
    }
    return 0;
}