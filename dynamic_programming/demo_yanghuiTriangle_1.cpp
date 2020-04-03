#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int yanghuiTriangle( vector<vector<int>> &items) {
    int states[items.size()][items.size()];

    states[0][0] = items[0][0];

    for (int i = 1; i < items.size(); i++) {
        for (int j = 0; j < items[i].size(); j++) {
            if (j == 0) {
                states[i][j] = states[i - 1][j] + items[i][j];
            } else if (j == items[i].size() - 1) {
                states[i][j] = states[i - 1][j - 1] + items[i][j];
            } else {
                int top1 = states[i - 1][j - 1];
                int top2 = states[i - 1][j];
                states[i][j] = min(top1, top2) + items[i][j];
            }
        }
    }

    for (int i = 0; i < items.size(); i++) {
        for (int j = 0; j < items.size(); j++) {
            cout << states[i][j] << " ";
        }
        cout << endl;
    }

    int minNum = 999;
    int len = items.size() - 1;
    for (int i = 0; i < len; i++) {
        if (states[len][i] < minNum) {
            minNum = states[len][i];
        }
    }
    return minNum;
}

int main() {
    vector<vector<int>> items = {
        {5},
        {7, 8},
        {2, 3, 4},
        {4, 9, 6, 1},
        {2, 7, 9, 4, 5}
    };
    
    int ret = yanghuiTriangle(items);
    cout << "结果为:" << ret << endl;;
    return 0;
}