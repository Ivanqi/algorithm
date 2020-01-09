#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 255

// 坏字符规则
// 构建散列表
void generateBC(char *p, int p_len, int *bc) {
    int i;
    for (i = 0; i < MAX_SIZE; i++) {
        bc[i] = -1;
    }
    for (i = 0; i < p_len; i++) {
        int ascii = (int)p[i];
        bc[ascii] = i;
    }
}

int BM(char *s, int s_len, char *p, int p_len) {
    printf("s_len:%d, p_len:%d\n", s_len, p_len);
    int bc[MAX_SIZE];               // 记录模式串中每个字符最后出现的位置
    generateBC(p, p_len, bc);       // 构建坏字符哈希表

    int i = 0;                      // i表示主串与模式串对齐的第一个字符
    while (i <= s_len - p_len) {
        int j;                      // j 为坏字符位置
        for (j = p_len - 1; j >= 0; j--) {  // 模式串从后往前匹配
            if (s[i + j] != p[j]) break;    // 坏字符对应模式串中的下标是j
        }
        if (j < 0) return i;                // 匹配成功，返回主串与模式串第一个匹配的字符位置
        printf("j:%d, i + j:%d, s[i + j]: %c, bc:%d\n", j, i + j, s[i + j], bc[(int) s[i + j]]);
        // bc[(int) s[i + j]]， 坏字符在bc的位置，如果没有返回 -1
        i = i + (j - bc[(int) s[i + j]]);
    }
    return -1;
}

int main() {

    char s[] = "ababcabcacbab";
    char p[] = "abcac";

    int number = BM(s, strlen(s), p, strlen(p));
    printf("pos:%d\n", number);
}