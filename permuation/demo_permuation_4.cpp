/**
 * 从n个数中选出m个数的方式，若不考虑顺序Cn(m), 若考虑顺序An(m)
 */

/**
 * 问题：密码排列
 * 假设有一个四位密码，每位密码是 a ~ e 之间的小写字
 * 编写密码可能排列方式
 */

#include <iostream>
#include <vector>
using namespace std;

class Permuation {
    private:
        int resultCount_ = 0;
        int maxResultLen_ = 4;
    public:
        /**
         * Details：根据输入字母列表，获取所有的排列方式
         * params: result- 当前排列方式， candidate- 未排列字母表
         * return: null
         */
        void breakPassword(vector<string> result, vector<string>candidate) {

            int len = candidate.size();
            int resultLen = result.size();
            
            if (maxResultLen_ == resultLen) {
                // 无字母剩余，输出排列结果
                outputResult(result);
                resultCount_++;
                return;
            }

            for (int i = 0; i < len; i++) {
                vector<string> resultNew;
                vector<string> candidateLeft;

                // 读取排列字母
                resultNew = result;
                resultNew.push_back(candidate[i]);

                // 获得剩余字母列表
                candidateLeft = candidate;
                vector<string>::iterator it = candidateLeft.begin();
                candidateLeft.erase(it + i);

                // 递归
                breakPassword(resultNew, candidateLeft);
            }
        }

        // 输出结果
        void outputResult(vector<string> result) {

            for (unsigned int i = 0; i < result.size(); i++) {
                cout << result[i];
            }
            cout << endl;
        }

        // 获取所有可能密码总数
        int getResultCount() {
            return resultCount_;
        }
};

int main (void) {

    vector<string> fourAlphabetString = {"a", "b", "c", "d", "e"};
    vector<string> res;
    Permuation test;
    test.breakPassword(res, fourAlphabetString);

    cout << "可能的密码形式:" ;
    cout << test.getResultCount() << "种" << endl;
}