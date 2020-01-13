#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getNext(char *p, int p_len, int *next) {
    next[0] = -1;
    int k = -1;
    int i;

    for (i = 1; i < p_len; ++i) {
        while (k != -1 && p[k + 1] != p[i]) {
            k = next[k];
        }
        if (p[k + 1] == p[i]) {
            ++k;
        }
        next[i] = k;
    }
}


int kmp(char *s, int s_len, char *p, int p_len) {
    int next[p_len];
    getNext(p, p_len, next);
    int j = 0;
    int i;

    for (i = 0; i < s_len; ++i) {
        while (j > 0 && s[i] != p[j]) { // 一直找到s[i] 和 p[j]
            j = next[j - 1] + 1;
        }

        if (s[i] == p[j]) ++j;
        
        if (j == p_len) {   // 找到匹配模式串
            return i - p_len + 1;
        }
    }

    return -1;
}
int main() {

    char s[] = "ababcabcacbab";
    char p[] = "abcac";

    int number = kmp(s, strlen(s), p, strlen(p));
    printf("pos:%d\n", number);
}