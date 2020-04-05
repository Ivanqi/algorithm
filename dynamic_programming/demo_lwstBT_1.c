#include <stdio.h>
#include <stdlib.h>

char a[] = "mitcmu";
char b[] = "mtacnu";
int n = 6;
int m = 6;
int minDist = 9999;

void lwstBT(int i, int j, int edist) {
    printf("i:%d, j:%d, edist:%d\n", i, j, edist);
    if (i == n || j == m) {
        if (i < n) edist += (n - i);
        if (j < m) edist += (m - j);
        if (edist < minDist) minDist = edist;
        return;
    }

    if (a[i] == b[j]) {                 // 两个字符匹配
        lwstBT(i + 1, j + 1, edist);
    } else {                            // 两个字符不匹配
        lwstBT(i + 1, j, edist + 1);    // 删除a[i] 或者 b[j]前添加一个字符
        lwstBT(i, j + 1, edist + 1);    // 删除b[j] 或者a[i]前添加一个字符
        lwstBT(i + 1, j + 1, edist + 1);    // 将a[i] 和 b[j]替换为相同字符
    }
}

int main() {

    lwstBT(0, 0, 0);
    printf("结果为:%d\n", minDist);
}