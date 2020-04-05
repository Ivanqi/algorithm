#include <stdio.h>
#include <stdlib.h>

#define printState(states, n, w) \
    for (int i = 0; i < n; ++i) { \
        for (int j = 0; j < w; ++j) { \
            printf("%d ", states[i][j]); \
        } \
        printf("\n"); \
    }

int lcs(char *a, int n, char *b, int m) {
    int states[n][m];
    int i, j;
    for (i = 0 ; i < n; i++) {  
        for (j = 0; j < m; j++) {
            states[i][j] = 0;
        }
    }

    for (i = 0; i < n; i++) {
        if (a[i] == b[0]) states[i][0] = i;
    }

    for (j = 0; j < m; j++) {
        if (b[j] == a[0]) states[0][j] = j;
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            int m1 = states[i - 1][j];
            int m2 = states[i][j - 1];
            int m3 = states[i - 1][j - 1];
            if (a[i] == b[j]) {
                m3 = m3 + 1;
                int max = (m1 > m3 ? (m1 > m2 ? m1 : m2) : (m3 > m2 ? m3 : m2));
                states[i][j] = max;
            } else {
                int max = (m1 > m3 ? (m1 > m2 ? m1 : m2) : (m3 > m2 ? m3 : m2));
                states[i][j] = max;
            }
        }
    }

    return states[n - 1][m - 1];
}

int main() {

    char a[] = "mitcmu";
    char b[] = "mtacnu";
    int n = 6;
    int m = 6;

    int ret = lcs(a, n, b, m); 

    printf("结果为:%d\n", ret);
}