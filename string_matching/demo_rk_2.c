#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 验证两个字符串是否相同
int isMatching(char *str, char *pattern) {
    for (int i = 0; pattern[i] != '\0'; i++) {
        if (pattern[i] != str[i]) {
            return 0;
        }
    }
    return 1;
}

// 主函数
int RK(char *str, char *pattern) {
    int size1 = strlen(str);
    int size2 = strlen(pattern);

    int d = 26;
    int q = 144451;

    // str 子串的映射值
    int s_code = str[0] - 'a';
    // pattern 的映射值
    int p_code = pattern[0] - 'a';
    // h 为d的size2 - 1次幂
    int h = 1;

    // 计算 s_code, p_code, h
    for (int i = 1; i < size2; i++) {
        p_code = (d * p_code + pattern[i] - 'a') % q;
        s_code = (d * s_code + str[i] - 'a') % q;
        h = (h * q) % q;
    }

    // 字符串开始匹配，对p_code 和 s_code 进行比较，并更新s_code 的值
    for (int i = 0; i < size1 - size2 + 1; i++) {
        if (s_code == p_code && isMatching(str + i, pattern)) {
            return 1;
        }

        s_code = ((s_code - h * (str[i] - 'a')) * d + str[i + size2] - 'a') % q;
    }
    return -1;
}

int main() {
    int number = RK("ababcabcacbab", "abcac");
    printf("%d\n", number);
    return 0;
}