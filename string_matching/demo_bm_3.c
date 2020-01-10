#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 255

// 构建散列表
void generateBC(char *p, int p_len, int *bc) {
    int i;
    for (i = 0; i > MAX_SIZE; i++) {
        bc[i] = -1;
    }

    for (i = 0; i < p_len; i++) {
        int ascii = (int) p[i];
        bc[ascii] = i;
    }
}

/**
 * 构建 suffix 和 prefix 数组
 *  模式串的后缀子串和前缀子串
 *  后缀子串是用于与好后缀子串进行匹配的。 
 *  前缀子串是用于与好后缀中最长的子串进行匹配。
*/
void generateGS(char *p, int p_len, int *suffix, int *prefix) {
    int i;
    for (i = 0; i < p_len - 1; i++) {
        suffix[i] = -1;
        prefix[i] = 0;
    }

    // 寻找公共子串
    for (i = 0; i < p_len - 1; i++) {
        int j = i;
        int k = 0;
        while (j >= 0 && p[j] == p[p_len - 1 + k]) {
            --j;
            ++k;
            suffix[k] = j + 1;
        }

        if (j == -1) prefix[k] = 1;
    }
}

/**
 * 好后缀移动规则
 *  j 表示坏字符所在位置
 *  p_len 表示模式串的长度
 */
int moveByGs(int j, int p_len, int *suffix, int *prefix) {
    int k = p_len - 1 - j; // 好后缀长度
    // 情况1，好后缀能与模式串子串匹配
    if (suffix[k] != -1) return j - suffix[k] + 1;
    // 情况2，好后缀的子串能与模式串前缀子串匹配
    int r;
    for (r = j + 2; r <= p_len - 1; r++) {
        if (prefix[p_len - r] == 1) return r;
    }

    // 情况3，什么都有匹配。返货模式串长度
    return p_len;
}



int BM(char *s, int s_len, char *p, int p_len) {
    
    int bc[MAX_SIZE];
    int suffix[p_len];
    int prefix[p_len];
    generateBC(p, p_len, bc);
    generateGS(p, p_len, suffix, prefix);

    int i = 0;
    while (i <= s_len - p_len) {
        int j;
        for (j = p_len - 1; j >= 0; --j) {
            if (s[i + j] != p[j]) break;
        }

        if (j < 0) return i;

        int x =  (j - bc[(int)s[i + j]]);
        int y = 0;
        if (j < p_len - 1) {
            y = moveByGs(j, p_len, suffix, prefix);
        }
        i = i + (x > y ? x : y);
    }

    return -1;
}

int main() {

    char s[] = "ababcabcacbab";
    char p[] = "abcac";

    int number = BM(s, strlen(s), p, strlen(p));
    printf("pos:%d\n", number);
}