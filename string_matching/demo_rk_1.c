#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define q 144451
#define d 26

int isMatch(char *S, int i, char *P, int m) {
    int is, ip;

    for (is = i, ip = 0; is != m && ip != m;is++, ip++) {
        if (S[is] != P[ip]) return 0;
    }

    return 1;
}

/**
 * 字符串匹配的RK算法
 * 若成功匹配返回主串中的偏移，否则返回-1
 */
int RK(char *S, char *P) {
    int m = strlen(P);
    int n = strlen(S);

    unsigned int h = 1;
    unsigned int A = 0;
    unsigned int St = 0;
    int i;

    // 初始化，算出最d进行下的最高位
    for (i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    for (i = 0; i != m; i++) {
        A = (d * A + (P[i] - 'a')) % q;
        St = (d * St + (S[i] - 'a')) % q;
    }

    for (i = 0; i != n - m; i++) {
        if (A == St) {
            if (isMatch(S, i, P, m)) return i;
        }
        St = (d * (St - h * (S[i] - 'a')) + (S[i + m])) % q;
    }

    return -1;
}

int main() {
    int number = RK("ababcabcacbab", "abcac");
    printf("%d\n", number);

    return 0;
}