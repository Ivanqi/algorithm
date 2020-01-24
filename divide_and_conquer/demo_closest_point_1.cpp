#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

const int N = 100005;
const double MAX = 10e100;
const double eps = 0.00001;

typedef struct TYPE {
    double x, y;
    int index;
} Point;

Point a[N], b[N], c[N];
double closest(Point *, Point *, Point *, int, int);
double dis(Point, Point);
int cmp_x(const void *, const void *);
int cmp_y(const void *, const void *);
int merge(Point *, Point *, int, int, int);
inline double min(double, double);

int main() {

    int n, i;
    double d;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%lf%lf", &(a[i].x), &(a[i].y));
    }
    qsort(a, n, sizeof(a[0]), cmp_x);
    for (i = 0; i < n; i++) {
        a[i].index = i;
    }
    memcpy(b, a, n * sizeof(a[0]));
    qsort(b, n, sizeof(b[0]), cmp_y);
    d = closest(a, b, c, 0, n - 1);
    printf("%.2fl\n", d);
    return 0;
}

double closest(Point a[], Point b[], Point c[], int p, int q) {
    if (q - p == 1) return dis(a[p], a[q]);

    if (q - p == 2) {
        double x1 = dis(a[p], a[q]);
        double x2 = dis(a[p + 1], a[q]);
        double x3 = dis(a[p], a[p + 1]);

        if (x1 < x2 && x1 < x3) {
            return x1;
        } else if (x2 < x3) {
            return x2;
        } else {
            return x3;
        }
    }

    int m = (p + q) / 2;
    int i, j, k;
    double d1, d2;

    for (i = p, j = p, k = m + 1; i <= q; i++) {
        if (b[i].index <= m) {
            c[j++] = b[i];
        } else {
            // 数组c左半边保存划分后左部的点，且对y是有序的
            c[k++] = b[i];
        }
    }
    d1 = closest(a, c, b, p, m);
    d2 = closest(a, c, b, m + 1, q);
    double dm = min(d1, d2);
    merge(b, c, p, m, q);   // 数组c左右部分分别是对y坐标有序的，将其合并到b

    for (i = p, k = p; i <= q; i++) {
        if (fabs(b[i].x - b[m].x) < dm) {
            c[k++] = b[i];
        }
    }

    // 找出离划分基准左右不超过dm的部分，且仍然对y坐标有序
    for (i = p; i < k; i++) {
        for (j = i + 1; j < k && c[j].y - c[i].y < dm; j++) {
            double temp = dis(c[i], c[j]);
            printf("temp:%f\n", temp);
            if (temp < dm) {
                dm = temp;
            }
        }
    }

    return dm;
}

double dis(Point p, Point q) {
    double x1 = p.x - q.x, y1 = p.y - q.y;
    printf("p.x:%f, q.x:%f, p.y:%f, q.y:%f, x1:%f, y1:%f, sqrt:%f\n", p.x, q.x, p.y, q.y, x1, y1, sqrt(x1 * x1 + y1 * y1));
    return sqrt(x1 * x1 + y1 * y1);
}

int merge(Point p[], Point q[], int s, int m, int t) {
    int i, j, k;
    for (i = s, j = m + 1, k = s; i <= m && j <= t;) {
        if (q[i].y > q[j].y) {
            p[k++] = q[j], j++;
        } else {
            p[k++] = q[i], i++;
        }
    }

    while (i <= m) {
        p[k++] = q[i++];
    }

    while (j <= t) {
        p[k++] = q[j++];
    }

    memcpy(q + s, p + s, (t - s + 1) * sizeof(p[0]));
    return 0;
}

int cmp_x(const void *p, const void *q) {
    double temp = ((Point*)p)->x - ((Point*)q)->x;
    if (temp > 0) {
        return 1;
    } else if (fabs(temp) < eps) {
        return 0;
    } else {
        return -1;
    }
}

int cmp_y(const void *p, const void *q) {
    double temp = ((Point*)p)->y - ((Point*)q)->x;
    if (temp > 0) {
        return 1;
    } else if (fabs(temp) < eps) {
        return 0;
    } else {
        return -1;
    }
}

inline double min(double p, double q) {
    return (p > q) ? (q) : (p);
}