#include <stdio.h>
#include <vector>
#include <limits.h>
#include <iostream>

using namespace std;
#define MIN(a, b) (a) > (b) ? (b) : (a)

// https://zhuanlan.zhihu.com/p/337385365
int minCoins(int money, const std::vector<int>& coins, std::vector<int>& dp) {
	int coinsLength = coins.size();

	for(int i = 1; i < money + 1; i++) {
		for(int j = 0; j < coinsLength; j++) {
			int v = coins[j];
			if (i < v) { continue; }
			dp[i] = MIN(dp[i], dp[i - v] + 1);
            cout << "money: " << i << " dp[" <<  i  << "]: " << dp[i] << endl;
		}
	}

	return dp[money];
}

int main() {

    int coinsArr[] = {1, 3, 5};
    std::vector<int> coins(coinsArr, coinsArr + sizeof(coinsArr) / sizeof(coinsArr[0]));

    int money = 9;

    std::vector<int> dp(money + 1, money + 1); // 备忘录，值初始化为k+1
    dp[0] = 0;

    int minCount = minCoins(money, coins, dp);
    printf(" minCount = %d\n", minCount);

    return 0;
}