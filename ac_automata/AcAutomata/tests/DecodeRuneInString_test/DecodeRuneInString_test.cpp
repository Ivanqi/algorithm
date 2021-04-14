#include "Unicode.h"
#include <stdio.h>

void test_case_1() {

    Unicode example = DecodeRunesInString("你");
    printf("example size: %d\n", (int)example.size());

    for (Unicode::const_iterator citer = example.begin(); citer != example.end(); ++citer) {
        printf("citer:%d\n", *citer);
    }
}

int main() {

    test_case_1();
    return 0;
}