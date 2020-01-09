#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 255

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
    int bc[MAX_SIZE];
    generateBC(p, p_len, bc);

    int i = 0;
    while (i <= s_len - p_len) {
        int j;
        for (j = p_len - 1; j >= 0; j--) {
            if (s[i + j] != p[j]) break;
        }
        if (j < 0) return i;
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