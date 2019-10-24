#include <stdio.h>
#include <stdlib.h>

// 查找最后一个小于等于给定值的元素

int binary_search(int a[], int n, int value) {
    
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + ((high - low) >> 1);
       
        if (a[mid] > value) {
            high = mid - 1;
        } else {
            if ((mid == n - 1) || (a[mid + 1] > value)) return mid;
            else low = mid + 1;
        }
    }
    return -1;
}

void test_case_1() {
    int n = 6;
    int a[] = {3, 5, 6, 8, 9, 10};
    int value = 7;

    printf("下标为： %d\n", binary_search(a, n, value));
}

int main() {
    test_case_1();
}