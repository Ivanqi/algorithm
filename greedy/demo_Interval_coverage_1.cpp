#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class IntervalCoverage 
{
    public:
        vector<vector<int>> intterval_coverage(vector<vector<int>>& intervals)
        {
            vector<vector<int>> ret;
            if (intervals.empty()) return ret;
            sort(intervals.begin(), intervals.end());

            int left = intervals[0][1];

            for (int i = 1; i < intervals.size(); i++) {
                if (intervals[i][0] < left) {
                    vector<int> tmp;
                    tmp.push_back(intervals[i][0]);
                    tmp.push_back(intervals[i][1]);
                    ret.push_back(tmp);
                    left = min(intervals[i][i], left);
                } else {
                    left = intervals[i][1];
                }
            }
            return ret;
        }
};

int main() {

    IntervalCoverage Interval;

    vector<int> num1 {6, 8};
    vector<int> num2 {2, 4};
    vector<int> num3 {3, 5};
    vector<int> num4 {1, 5};
    vector<int> num5 {5, 9};
    vector<int> num6 {8, 10};


    vector<vector<int>> intervals;
    intervals.push_back(num1);
    intervals.push_back(num2);
    intervals.push_back(num3);
    intervals.push_back(num4);
    intervals.push_back(num5);
    intervals.push_back(num6);

    vector<vector<int>> ret = Interval.intterval_coverage(intervals);
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

