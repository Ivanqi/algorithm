#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

vector<int> Num {3, 0, 2, 1, 0, 3, 5}, Value {1, 2, 5, 10, 20, 50, 100};

vector<tuple<int, int>> coinChage(int money) {
    int sum = 0;
    vector<tuple<int, int>> ch;

    for (int i = Value.size() - 1; i >= 0; --i) {
        int N = min(money / Value[i], Num[i]);
        money = money - N * Value[i];
        sum += N;
        if (N != 0) {
            ch.push_back({Value[i], N});
        }

        if (money == 0) return ch;
    }

    ch.clear();
    ch.push_back({-1, -1});
    return ch;
}

int main() {

    int money;
    cin >> money;
    vector<tuple<int, int> > m = coinChage(money);

    for (int i = 0; i < m.size(); ++i) {
        cout << get<0>(m[i]) << ":" << get<1>(m[i]) << endl;
    }

    return 0;
}