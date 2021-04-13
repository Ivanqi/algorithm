#include <iostream>
#include <string>

using namespace std;

const int maxNum = 1005;

int shift[maxNum];
int Sunday(const string& T, const string& P) {

    int n = T.length();
    int m = P.length();

    // 默认值，移动 m + 1位
    for (int i = 0; i < maxNum; i++) {
        shift[i] = m + 1;
    }

    /**
     * 模式串P中每个字母出现的最后的下标
     * 所对应的主串参与匹配的最末位字符的下一位字符移动移动到该位，所需要的移动位数
     */
    for (int i = 0; i < m; i++) {
        shift[P[i]] = m - i;
    }

    // 模式串开始位置在主串的哪里
    int s = 0;
    int tmp = 0;

    // 模式串已经匹配到的位置
    int j;
    while (s <= n - m) {
        j = 0;

        while (T[s + j] == P[j]) {
            j++;
            // 匹配成功
            if (j >= m) {
                return s;
            }
        }

        /**
         * 找到主串中当前跟模式串匹配的最末字符的下一个字符
         * 在模式串中出现最后的位置
         * 所需要从(模式串末尾+1)移动到该位置的步数
         */
        tmp = shift[T[s + m]];
        s += tmp;
    }

    return -1;
}

int test_case_1() {
    // 主串和模式串
    string T("at the thought of"), P("though");
    int res = Sunday(T, P);
    return res;
}

int test_case_2() {
    // 主串和模式串
    string T("substring searching"), P("search");
    int res = Sunday(T, P);
    return res;
}


int main() {

    int res = test_case_2();
    
    if (res == -1) {
        cout << "主串和模式串不匹配." << endl;
    } else {
        cout << "模式串在主串的位置为: " << res << endl;
    }
    return 0;
}