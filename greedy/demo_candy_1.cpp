#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 题目
 *  已知一些孩子和一些糖果，每个孩子有需求因子g,每个糖果有大小s，当某个糖果的大小s >= 某个孩子的需求因子g时，代表该糖果可以满足该孩子
 *  求使用这些糖果，最多能满足多少孩子？(注意，某个孩子最好只能用1个糖果满足)
 *  例如：需求因子数组g = [5, 10, 2, 9, 15, 9];糖果大小数组s = [6, 1, 20, 3, 8];最多可以满足3个孩子
 * 题解
 *  需求因子数组g = [2, 5, 9, 9, 10, 15];糖果大小数组s = [1, 3, 6, 8, 20];
 *  核心目标：让更多孩子得到满足
 *  规则
 *      1. 某个糖果如果不能满足某个孩子，则该糖果也一定不能满足需求因子更大的孩子
 *         糖果1(s = 1)不能满足孩子1(g = 2),则不能满足孩子2，孩子3，..., 孩子6
 *         糖果1(s = 3)不能满足孩子2(g = 5),则不能满足孩子2，孩子3，..., 孩子6
 *      2. 某个孩子可以用更小的糖果满足，则没必要用更大糖果满足，因为可以保留更大的糖果满足需求更大的孩子
 *         孩子1(g = 2)，可以被糖果2(s = 3)满足，则没必要用糖果3，糖果4，糖果5满足
 *         孩子2(g = 2), 可以被糖果3(s = 6)满足，则没必要用糖果4，糖果5满足
 *      3. 孩子的需求因子更小则其更容易满足，故优先从需求因子小的孩子尝试，用某个糖果满足一个较大需求因子的孩子或满足一个较小需求因子孩子效果一样的
 *         对需求因子数组g与糖果大小数组s进行从小到大排序
 *         按照从小到大的顺序使用各糖果尝试是否满足某个孩子，每个糖果只尝试1次；若尝试成功，则换下一个孩子尝试；直到发现没有更多孩子或更多糖果，循环结束
 */        

class MyClass
{
    public:
        int getMinResult(vector<int>& g, vector<int>& s) {
            // 进行排序
            sort(g.begin(), g.end());
            sort(s.begin(), s.end());

            int child = 0;
            int cookie = 0;

            while (child < g.size() && cookie < s.size()) {
                if (s[cookie] >= g[child]) {
                    child++;
                }
                cookie++;
            }
            return child;
        }
};

int main() {
    vector<int> g;
    vector<int> s;

    g.push_back(5);
    g.push_back(10);
    g.push_back(2);
    g.push_back(9);
    g.push_back(15);
    g.push_back(9);
    s.push_back(6);
    s.push_back(1);
    s.push_back(20);
    s.push_back(3);
    s.push_back(8);

    MyClass cls;
    int n =cls.getMinResult(g, s);
    cout << n << endl;
}