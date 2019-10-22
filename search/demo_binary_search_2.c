#include <stdio.h>
#include <stdlib.h>

/**
 * mid 取值
 *  1. 实际上，mid = (low + high) / 2 这种写法是有问题的，因为如果low和high比较大的话，两者之和就可能会溢出
 *  2. 改进的方法是将mid的计算方式写成 low + (high - low) / 2
 *  3. 如果要将性能优化到极致的话，我们可以将这里的除以2操作转化成位运算 low + ((high - low) >> 1).因为相比除法运算来说，计算机处理位运算要快得多
 */
int binary_search(int a[], int low, int high, int value) {
    if (low > high) return -1;

    int mid = low + ((high - low) >> 1);
    if (a[mid] == value) {
        return mid;
    } else if (a[mid] < value) {
        return binary_search(a, mid + 1, high, value);
    } else {
        return binary_search(a, low, mid - 1, value);
    }
}

void test_case_1() {
    int n = 7;
    int a[] = {8, 11, 19, 23, 27, 33, 45};
    int value = 23;

    printf("下标为： %d\n", binary_search(a, 0, n - 1, value));
}

void test_case_2() {
    int n = 7;
    int a[] = {9, 14, 19, 23, 27, 33, 45};
    int value = 15;

    printf("下标为： %d\n", binary_search(a, 0, n - 1, value));
}

int main() {
    test_case_1();
}