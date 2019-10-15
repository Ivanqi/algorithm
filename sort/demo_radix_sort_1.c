#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define NUM_OF_POS(a, pval) ((a) / pval) % 10

void dump(int *arr, int size) {
    int idx;
    for (idx = 0; idx < size; idx++) {
        printf("%d\n", arr[idx]);
    }
}

/**
select (x / pow(10, 0)) % 10;
123	12341	124	236	128	1112313129	98	9	8989
3	1		4	6	8	9			8	9	9


select (x / pow(10, 1)) % 10;
123	12341	124	236	128	1112313129	98	9	8989
2	4		2	3	2	2			9	0	8

select (x / pow(10, 2)) % 10;
123	12341	124	236	128	1112313129	98	9	8989
1	3		1	2	1 	1			0	0	9


select (x / pow(10, 3)) % 10;
123	12341	124	236	128	1112313129	98	9	8989
0	2		0	0	0	3			0	0	8

select (x / pow(10, 4)) % 10;
123	12341	124	236	128	1112313129	98	9	8989
0	0		0	0	0	1			0	0	0

select (x / pow(10, 5)) % 10;
123	12341	124	236	128	1112313129	98	9	8989
0	0		0	0	0	3			0	0	0

.....
 */

void radix_sort(int a[], int size, int num_count) {
    int count[10] = {0};    // 计数
    int *pres = NULL;
    int i = 0;
    int j = 0;
    int pval = 0;
    int index = 0;
    int break_flg = 0;

    pres = (int *)malloc(sizeof(int) * size);
    assert(pres != NULL);

    for (i = 0; i < num_count; i++) {
        memset(count, 0, sizeof(int) * 10);

        // 求当前的基数
        pval = pow(10, i);
        // 计数
        for (j = 0; j < size; j++) {
            index = NUM_OF_POS(a[j], pval);
            count[index]++;
        }

        if (count[0] == 9) {
            break_flg++;
        }

        if (break_flg >= 2) {
            printf("\n %i", i);
            break;
        }

        // 累加
        for (j = 1; j < 10; j++) {
            count[j] += count[j - 1];
        }

        // 排序必须从后往前，否则不是稳定排序
        for (j = size - 1; j >= 0; j--) {
            index = NUM_OF_POS(a[j], pval);
            pres[count[index] - 1] = a[j];
            count[index]--; 
        }

        // 本轮排序好的，拷贝到a中
        memcpy(a, pres, sizeof(int) * size);
    }

    return;
}

void radix_sort_test() {
    int a[10] = {123, 12341, 12341, 124, 236, 128, 1112313129, 98, 9, 8989};
    printf("\n radix sort test .... ");

    radix_sort(a, 10, 10);
    dump(a, 10);
    return;
}

int main() {
    radix_sort_test();
    return 0;
}