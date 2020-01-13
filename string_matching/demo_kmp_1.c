#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GetNext(char *p, int *next) {
    int pLen = strlen(p);
    next[0] = -1;
    int k = -1;
    int j = 0;

    while (j < pLen - 1) {
        // p[k]表示前缀，p[j]表示后缀
        if (k == -1 || p[j] == p[k]) {
            ++k;
            ++j;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
}

int KmpSearch(char *s, char *p) {
    int i = 0;
    int j = 0;
    int sLen = strlen(s);
    int pLen = strlen(p);
    int next[pLen];
    GetNext(p, next);

    while (i < sLen && j < pLen) {
        // 如果 j == -1，或者当前字符匹配成功(即S[i] == P[j]),都会i++,j++
        if (j == -1 || s[i] == p[j]) {
            i++;
            j++;
        } else {
            // 如果j != -1,且当前字符匹配失败(即S[i] != P[j]),则令i不变，j = next[j]
            // next[j]即为j所对应的next值
            j = next[j];
        }
    }

    if (j == pLen) {
        return i - j;
    } else {
        return -1;
    }
}

int main() {

    char s[] = "ababcabcacbab";
    char p[] = "abcac";

    int number = KmpSearch(s, p);
    printf("pos:%d\n", number);
}