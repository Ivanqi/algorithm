#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * 每次选择的时候，左端点跟前面的已经覆盖的区间不重合的，右端点又尽量尽量小
 * 这样可以让剩下的未覆盖区间尽可能大，就可以放置更多区间。实际上就是一种贪心的选择方法
 */

vector<vector<int>> intterval_coverage(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> ret;
    int left = 0;

    for (int i = 0; i < intervals.size(); i++) {
        if (intervals[i][0] < left) {
            ret.push_back(intervals[i]);
            left = min(intervals[i][0], left);
        } else {
            left = intervals[i][1];
        }
    }

    return ret;
}

vector<vector<int>> test_case_1() {
    vector<vector<int>> intervals{
        {6, 8},
        {2, 4},
        {3, 5},
        {1, 5},
        {5, 9},
        {8, 10}
    };

    vector<vector<int>> ret = intterval_coverage(intervals);

    return ret;
    
}

vector<vector<int>> test_case_2() {
    vector<vector<int>> intervals{
        {2, 6},
        {1, 4},
        {3, 6},
        {3, 7},
        {6, 8},
        {2, 4},
        {3, 5}
    };

    vector<vector<int>> ret = intterval_coverage(intervals);

    return ret;
    
}

int main() {
    vector<vector<int>> ret = test_case_1();

    cout << "size: " << ret.size() << endl;

    if (ret.size() > 0) {
        cout << "[ \n";
        for (int i = 0; i < ret.size(); i++) {
            vector<int> tmp = ret[i];
            cout << " [";
            for (int j = 0; j < tmp.size(); j++) {
                cout << tmp[j] << " ";
            }
            cout << "] \t";
        }
        cout << " \n]" << endl;
    }
    return 0;
}