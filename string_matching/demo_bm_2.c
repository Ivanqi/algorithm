#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 好后缀规则

/**
 *  模式串的后缀子串和前缀子串
 *  后缀子串是用于与好后缀子串进行匹配的。 
 *  前缀子串是用于与好后缀中最长的子串进行匹配。
*/
void generateGS(char *p, int p_len, int *suffix, int *prefix) {
    // 初始化
    int i;
    for (i = 0; i < p_len; i++) {
        suffix[i] = -1;
        prefix[i] = 0;
    }

    for (i = 0; i < p_len - 1; i++) {                // b[0, i]
        int j = i;
        int k = 0;                                   // 公共后缀子串长度

        while (j >= 0 && p[j] == p[p_len - 1 - k]) { // 与b[0, m - 1]求公共后缀子串
            --j;
            ++k;
            suffix[k] = j + 1;                       // j + 1 表示公共后缀子串在b[0, i] 中的起始下标
        }
        if (j == -1) prefix[k] = 1;                 // 如果公共后缀子串也是模式的前缀子串
    }
}

/**
 * j 表示坏字符所在位置
 * p_len 表示模式串的长度
 */
int moveByGS(int j, int p_len, int *suffix, int *prefix) {

    int k = p_len - 1 - j;  // 好后缀长度
    // 情况1，suffix中存在好后缀
    if (suffix[k] != -1) return j - suffix[k] + 1;
    // 情况2，查找好后缀的最长字符串是否和prefix的前缀子串匹配
    int r;
    // j是坏字符，j+1是好后缀，j+2是好后缀的后缀子串的起始
    for (r = j + 2; r <= p_len - 1; r++) {
        if (prefix[p_len - r] == 1) return r;
    }
    // 情况3， 什么情况都没有匹配.返回模式串的长度
    return p_len;
}

int BM(char *s, int s_len, char *p, int p_len) {

    int suffix[p_len];
    int prefix[p_len];
    generateGS(p, p_len, suffix, prefix);

    int i = 0;                              // i表示主串与模式串对齐的第一个字符
    while (i <= s_len - p_len) {
        int j;                              // j 为坏字符位置
        for (j = p_len - 1; j >= 0; j--) {  // 模式串从后往前匹配
            if (s[i + j] != p[j]) break;    // 坏字符对应模式串中的下标是j
        }
        if (j < 0) return i;                // 匹配成功，返回主串与模式串第一个匹配的字符位置
        if (j < p_len - 1) {
            i = i + moveByGS(j, p_len, suffix, prefix);
        } else {
            i = i + 0;
        }
    }
    return -1;
}

int main() {

    char s[] = "ababcabcacbab";
    char p[] = "abcac";

    int number = BM(s, strlen(s), p, strlen(p));
    printf("pos:%d\n", number);
}