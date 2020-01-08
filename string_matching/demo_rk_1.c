#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isMatching(char *str, char *pattern) {
    int i;
    for (i = 0; pattern[i] != '\0'; i++) {
        if (pattern[i] != str[i]) {
            return 0;
        }
    }
    return 1;
}

// 主函数
int RK(char *str, char *pattern) {
    int d = 26;

    int size1 = strlen(str);
    int size2 = strlen(pattern);

    int p_code = 0;
    int s_code = 0;

    int i, j, k;
    for (k = 0, i = size2; i > 0; i--, k++) {
        int num = 1;
        for (j = 0; j < (i - 1); j++) {
            num = num * d;
        }
        p_code = (num * (pattern[k] - 'a')) + p_code;
        s_code = ((num * (str[k] - 'a')) + s_code);
    }
    
    for (i = 1; i <= size1 - size2 + 1; i++) {
        if (s_code == p_code && isMatching(str + i - 1, pattern)) {
            printf("hit !. start:%d, end: %d\n", i - 1, i - 2 + size2);
            return 1;
        }
        k = 1;
        for (j = 0; j < size2 - 1; j++) {
            k = k * d;
        }
        s_code = (s_code - k * (str[i - 1] - 'a') ) * d + (str[i + size2 - 1] - 'a');
    }
    return -1;
}

int main() {
    int ret = RK("ababcabcacbab", "abcac");
    printf("%d\n", ret);

    return 0;
}