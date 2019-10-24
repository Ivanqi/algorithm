#include <stdio.h>
#include <stdlib.h>

// 查找最后一个值等于给定值的元素

int binary_search(int a[], int n, int value) {
    
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + ((high - low) >> 1);
        if (a[mid] > value) {
            high = mid - 1;
        } else if (a[mid] < value) {
            low = mid + 1;
        } else {
            /**
             * 如果a[mid]这个元素已经是数组中的最后一个元素了，那它肯定是我们要找的
             * 如果a[mid]的后一个元素a[mid + 1]不等于value，那也说明 a[mid]就是我们要找的最后一个值等于给定值的元素
             * 
             * 如果我们经过检查之后，发现a[mid]后面的一个元素a[mid + 1] 也等于value，那说明当前的这个a[mid]并不是最后一个值等给定值的元素
             * 我们就更新low = mid + 1，因为要找的元素肯定出现在[mid + 1, high]之间
             */
            if ((mid == n - 1) || (a[mid + 1] != value)) return mid;
            else low = mid + 1;
        }
    }
    return -1;
}

void test_case_1() {
    int n = 12;
    int a[] = {1, 3, 4, 5, 6, 8, 8, 8, 8, 11, 18};
    int value = 8;

    printf("下标为： %d\n", binary_search(a, n, value));
}

int main() {
    test_case_1();
}