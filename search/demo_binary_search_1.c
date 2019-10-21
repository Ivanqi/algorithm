#include <stdio.h>
#include <stdlib.h>

int binary_search(int a[], int n, int value) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (a[mid] == value) {
            return mid;
        } else if (a[mid] < value) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

void test_case_1() {
    int n = 7;
    int a[] = {8, 11, 19, 23, 27, 33, 45};
    int value = 23;

    printf("下标为： %d\n", binary_search(a, n, value));
}

void test_case_2() {
    int n = 7;
    int a[] = {9, 14, 19, 23, 27, 33, 45};
    int value = 15;

    printf("下标为： %d\n", binary_search(a, n, value));
}

int main() {
    test_case_2();
}