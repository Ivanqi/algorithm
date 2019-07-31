#include <iostream>
#include <string>
#include <vector>
using namespace std;

/***
 * 1. C++ 实现，对总金额100的最小纸币是15
 * 2. 用递归法总额为30就要很久
 * 3. 另外的数学办法可以用总金额依次对最大金额求余数，直到为0.商相加为答案。 如：若{1,2,3,7}为纸币金额，对于100，所需最小纸币数：100 / 7 = 14余2
 * 4. 2 / 2 =1 余0.则纸币数为14 + 1 = 15
 */
class DynamicProgramming {
    private:
        vector<int> money = {1, 2, 3, 7}; //纸币种类

    public:
        /***
         * Desciption: 对于金额固定，找出最少钱币数以及方法
         * params: amountMoeny- 输入总金额
         * return: 所需最小纸币数
         */
        int finddFewersMethod (int amountMoeny) {
            int c[amountMoeny];
            c[0] = 0;

            int temp;
            for (unsigned int i = 1; i < amountMoeny; i++) {
                // 用最大值初始化
                int tempMin = amountMoeny;
                for (unsigned int j = 0; j < money.size(); j++) {
                    int diff = i - money[j];
                    if (0 <= diff) {
                        temp = c[diff] + 1;
                    } else {
                        // 此情况表示纸币无效，选择最大值
                        temp = amountMoeny;
                    }
                    // 求出最小值
                    if (temp < tempMin) {
                        tempMin = temp;
                    }
                }
                c[i] = tempMin;
            }
            return c[amountMoeny - 1];
        } 
};

// test 
int main (void) {
    DynamicProgramming test;
    int res = test.finddFewersMethod(25);
    cout << res << endl;
    return 0;
}