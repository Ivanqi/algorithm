#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI 3.141592654

/**
 * 参考资料
 *  http://www.cnblogs.com/tsingke/p/6194737.html
 *  https://blog.csdn.net/u012480384/article/details/50838832
 */
double gaussrand() {
    static double U, V;
    static int phase = 0;
    double Z;

    if (phase == 0) {
        U = rand() / (RAND_MAX + 1.0);
        V = rand() / (RAND_MAX + 1.0);
        Z = sqrt(-2.0 * log(U)) * sin(2.0 * PI * V);
    } else {
        Z = sqrt(-2.0 * log(U)) * cos(2.0 * PI * V);
    }

    phase = 1 - phase;
    return Z;
}

int main() {

    for (int i = 0; i < 10; i++) {
        double ret = gaussrand();
        printf("结果为: %lf\n", ret);
    }
    return 0;
}