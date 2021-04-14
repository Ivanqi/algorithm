#include "Unicode.h"
#include <stdio.h>

Unicode test_case_1() {

    Unicode tmp = DecodeRunesInString("你");
    return tmp;
}

Unicode test_case_2() {

    Unicode tmp = DecodeRunesInString("你好，我来测试的");
    return tmp;
}

int main() {

    Unicode ret = test_case_2();
    printf("ret size: %d\n", (int)ret.size());

    for (Unicode::const_iterator citer = ret.begin(); citer != ret.end(); ++citer) {
        printf("citer:%d\n", *citer);
    }

    return 0;
}