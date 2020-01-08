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

    //质数表对应 a - z
    int table[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101}; 
    int i, j, hash_val;
    int value;

    for (i = 0; i < size2; ++i) {   // 计算模式串的hash值value
        value += table[pattern[i] - 'a'];
    }

    for (i = 0; i < size1 - size2 + 1; ++i) {   // 最多n - m + 1次比较
        hash_val = 0;
        for (j = i; j < size2 + i; ++j) {   // 计算第i个子串的哈希值
            hash_val += table[str[j] - 'a'];
        }

        if (hash_val == value && isMatching(str + i, pattern)) {
            printf("hit !. start:%d, end: %d\n", i, i - 1 + size2);
            return 1;
        }
    }
    return -1;
}

int main() {
    int number = RK("ababcabcacbab", "abcac");
    printf("%d\n", number);
    return 0;
}