#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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