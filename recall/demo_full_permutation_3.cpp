#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 解决一个回溯问题，实际上就是一个决策树的遍历过程。只需要思考3个问题
 *  1. 路径：也就是已经做出的选择
 *  2. 选择列表：也就是你当前可以做的选择
 *  3. 结束条件：也就是到达决策树底层，无法再做选择的条件
 */
class Solution {
    public:
        vector<vector<int>> ret;

        // 主函数，输入一组不重复的数字，返回它们的全排列
        vector<vector<int>> permute(vector<int>& nums) {
            // 记录路径
            vector<int> track;
            backtrack(nums, track);
            return ret;
        }

        /**
         * 路径：记录在track中
         * 选择列表：nums中不存在于track的那些元素
         * 结束条件：nums中的全部元素都在track中出现
         */
        void backtrack(vector<int>& nums, vector<int>& track) {
            int numsLen = nums.size();
            if (track.size() == numsLen) {
                ret.push_back(track);
                return ;
            }

            for (int i = 0; i < numsLen; i++) {
                // 排除不合法的选择
                if (find(track.begin(), track.end(), nums[i]) == track.end()) {
                    // 做选择
                    track.push_back(nums[i]);
                    // 进入下一个决策树
                    backtrack(nums, track);
                    // 取消选择
                    track.pop_back();
                }
            }
        }
};


void test_case_1(Solution So) {

    vector<int> nums;
    nums.push_back(1);
    nums.push_back(2);
    nums.push_back(3);

    vector<vector<int>> ret = So.permute(nums);

    cout << "[ \n";
    for (int i = 0; i < ret.size(); i++) {
        vector<int> retn = ret[i];
        cout << "\t[ ";
        for (int j = 0; j < retn.size(); j++) {
            cout << retn[j] << " ";
        }
        cout << "]\n";
    }
     cout << "\n]";
}

int main () {

    Solution So;

    test_case_1(So);
    return 0;
}