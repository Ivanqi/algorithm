#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NSUM 25

// 参考资料: http://www.cnblogs.com/tsingke/p/6194737.html
// 大数定理
double gaussrand() {
    double x = 0;
    int i;
    for (i = 0; i < NSUM; i++) {
        x += (double) rand() / RAND_MAX;
    }

    x -= NSUM / 2.0;
    x /= sqrt(NSUM / 12.0);

    return x;
}

int main() {

    for (int i = 0; i < 10; i++) {
        double ret = gaussrand();
        printf("结果为: %lf\n", ret);
    }
    return 0;
}