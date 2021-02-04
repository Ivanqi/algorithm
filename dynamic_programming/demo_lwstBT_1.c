#include <stdio.h>
#include <stdlib.h>

char a[] = "mitcmu";
char b[] = "mtacnu";
int n = 6;
int m = 6;
int minDist = 9999;

/**
 * 编辑距离指的是：将一个字符串转化成另一个字符串，需要的最少编辑操作次数(比如增加一个字符、删除一个字符、替换一个字符)
 * 编辑距离越大，说明两个字符串的相似程序越小。相反，编辑距离越小，说明两个字符串的相似程度越大
 * 对于完全相同的字符来说，编辑距离就是0
 */
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