#include <stdio.h>
#include <stdlib.h>

// 查找第一个值等于给定值的元素

int binary_search_1(int a[], int n, int value) {
    
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + ((high - low) >> 1);
        if (a[mid] >= value) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (low < n && a[low] == value) return low;
    else return -1;
}

int binary_search_2(int a[], int n, int value) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + ((high - low) >> 1);
        if (a[mid] > value) {
            high = mid - 1;
        } else if (a[mid] < value) {
            low = mid + 1;
        } else {
            if ((mid == 0) || (a[mid - 1] != value)) return mid;
            else high = mid - 1;
        }
    }

    return -1;
}

void test_case_1() {
    int n = 12;
    int a[] = {1, 3, 4, 5, 6, 8, 8, 8, 8, 11, 18};
    int value = 8;

    printf("下标为： %d\n", binary_search_1(a, n, value));
}

void test_case_2() {
    int n = 12;
    int a[] = {1, 3, 4, 5, 6, 8, 8, 8, 8, 11, 18};
    int value = 8;

    printf("下标为： %d\n", binary_search_2(a, n, value));
}

int main() {
    test_case_2();
}