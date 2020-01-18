#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> Num {3, 0, 2, 1, 0, 3, 5}, Value {1, 2, 5, 10, 20, 50, 100};

int coinChage(int money) {
    int sum = 0;
    for (int i = Value.size() - 1; i >= 0; --i) {
        int N = min(money / Value[i], Num[i]);
        // cout << Value[i] << '/' << money<< ":" << N << endl;
        money = money - N * Value[i];
        sum += N;
        if (money == 0) return sum;
    }
    return -1;
}

int main() {

    int money;
    cin >> money;
    int m = coinChage(money);

    cout << m << endl;

    return 0;
}