#include <stdio.h>
#include <string.h>

// BF算法实现函数。B 是主串，A是模式串
int mate(char *B, char *A) {
    int i = 0, j = 0;

    while (i < strlen(B) && j < strlen(A)) {
        if (B[i] == A[j]) {
            i++;
            j++;
        } else {
            i = i - j + 1;
            j = 0;
        }
    }

    // 跳出循环有两种可能，i = strlen(B)说明已经遍历完主串，匹配失败
    // j = strlen(A)， 说明子串遍历完成，在主串成功匹配
    if (j == strlen(A)) {
        return i - strlen(A) + 1;
    }

    // 运行到此，为i == strlen(B)的情况
    return 0;
}

int main() {
    int number = mate("ababcabcacbab", "abcac");
    printf("%d\n", number);

    return 0;
}