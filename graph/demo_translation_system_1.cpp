#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;

class translationSystem
{
    public:
        vector<int> A = {8, 6, 3};
        vector<int> B = {6, 5, 4, 2};
        vector<int> C = {9, 6};

        int maxNums = INT_MIN;

        void helper(int aSize, int bSize, int cSize) {
            if (bSize > B.size()) return;
            if (cSize > C.size()) return;

            int nums = A[aSize] + B[bSize] + C[cSize];
            if (nums > maxNums) maxNums = nums;

            helper(aSize, bSize, cSize + 1);
            helper(aSize, bSize + 1, cSize);
        }

        void calc() {
            for (int i = 0; i < A.size(); i++) {
                helper(i, 0, 0);
            }

            cout << "最大值为:" << maxNums << endl;
        }
};

int main() {
    translationSystem t;

    t.calc();
}