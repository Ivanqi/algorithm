#include <iostream>
#include <stdio.h>
using namespace std;

class Pattern {
    public:
        bool matched = false;
        char *pattern;  // 正则表达式
        int plen;       // 正则表达式长度

        Pattern(char* pattern, int plen): pattern(pattern), plen(plen) 
        {
        }

        bool match(char *text, int tlen) // 文本串及长度
        {
            matched = false;
            rmatch(0, 0, text, tlen);
            return matched;
        }

        void rmatch(int ti, int pj, char *text, int tlen) {
            printf("ti:%d, pj:%d\n", ti, pj);
            if (matched) return ;                   // 如果匹配了，就不要继续递归了
            if (pj == plen) {                       // 正则表达式到结尾了
                if (ti == tlen) matched = true;     // 文本串也到结尾了
                return;
            }

            if (pattern[pj] == '*') {               // *匹配任意个字符
                for (int k = 0; k <= tlen - ti; ++k) {
                    rmatch(ti + k, pj + 1, text, tlen);
                }
            } else if (pattern[pj] == '?') {        // ?匹配0个或者1个字符
                rmatch(ti, pj + 1, text, tlen);
                rmatch(ti + 1, pj + 1, text, tlen);
            } else if (ti < tlen && pattern[pj] == text[ti]) {  // 纯字符匹配才行
                rmatch(ti + 1, pj + 1, text, tlen);
            }
        }
    };

void test_case_1() {

    int plen = 2;
    char pattern[] = "a*";

    char text[] = "abc";
    int tlen = 3;

    Pattern p(pattern, plen);
    bool ret = p.match(text, tlen);
    cout << "匹配结果:" << ret << endl;

}

void test_case_2() {
    int plen = 6;
    char pattern[] = "c*a*b*";

    char text[] = "aab";
    int tlen = 3;

    Pattern p(pattern, plen);
    bool ret = p.match(text, tlen);
    cout << "匹配结果:" << ret << endl;
}

void test_case_3() {
    int plen = 5;
    char pattern[] = "a*c?b";

    char text[] = "acdcb";
    int tlen = 5;

    Pattern p(pattern, plen);
    bool ret = p.match(text, tlen);
    cout << "匹配结果:" << ret << endl;
}

int main() {
    test_case_3();
}