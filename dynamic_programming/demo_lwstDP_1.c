#include <stdio.h>
#include <stdlib.h>

#define printState(states, n, w) \
    for (int i = 0; i < n; ++i) { \
        for (int j = 0; j < w; ++j) { \
            printf("%d ", states[i][j]); \
        } \
        printf("\n"); \
    }

int lwstDP(char *a, int n, char *b, int m) {

    int states[n][m];
    int i, j;
    for (i = 0 ; i < n; i++) {  
        for (j = 0; j < m; j++) {
            states[i][j] = 0;
        }
    }
    
    for (i = 0; i < n; i++) {   // 初始化第0列: a[0...i]与b[0..0]的编辑距离 
        if (a[i] == b[0]) states[i][0] = i;
        else if (i != 0) states[i][0] = states[i - 1][0] + 1;
        else
            states[i][0] = 1;
    }

    for (j = 0; j < n; j++) {   // 初始化第0行：a[0..0]与b[0..j]的编辑距离
        if (b[j] == a[0]) states[0][j] = j;
        else if (j != 0) states[0][j] = states[0][j - 1] + 1;
        else states[0][j] = 1;
    }

    // printState(states, n, m);

    for (i = 1; i < n; i++) {
        for (j = 1; j < m; j++) {
            int m1 = states[i - 1][j] + 1;
            int m2 = states[i][j - 1] + 1;
            if (a[i] == b[j]) {
                int m3 = states[i - 1][j - 1];
                int min = (m1 < m3 ? (m1 < m2 ? m1 : m2) : (m3 < m2 ? m3 : m2));
                states[i][j] = min;
            } else {
                int m3 = states[i - 1][j - 1] + 1;
                int min = (m1 < m3 ? (m1 < m2 ? m1 : m2) : (m3 < m2 ? m3 : m2));
                states[i][j] = min;
            }
        }
    }
    printState(states, n, m);
    return states[n - 1][m -1];
}

int main() {

    char a[] = "mitcmu";
    char b[] = "mtacnu";
    int n = 6;
    int m = 6;

    int ret = lwstDP(a, n, b, m);
    printf("结果为:%d\n", ret);
    return 0;
}