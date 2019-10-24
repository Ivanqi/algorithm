#include <stdio.h>
#include <stdlib.h>

// 查找第一个大于等于给定值的元素

int binary_search(int a[], int n, int value) {
    
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + ((high - low) >> 1);
        /**
         * 如果a[mid]小于要查找的值value，那要查找的值肯定在[mid + 1, high]之间，所有我们要更新 low = mid + 1
         * 
         * 对于a[mid]大于等于给定值value的情况，我们要先看下这个a[mid]是不是我们要找的第一个值大于等于给定值的元素
         * 如果a[mid]前面已经没有元素，或者前面一个元素小于要查找的值value，那a[mid]就是我们要找的元素
         * 
         * 如果a[mid - 1]也大于等于要查找的值value，那说明查找的元素在[low, mid - 1]之间，所有，我们将high更新为 mid - 1
         */
        if (a[mid] >= value) {
            if ((mid == 0) || (a[mid - 1] < value)) return mid;
            else high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return -1;
}

void test_case_1() {
    int n = 5;
    int a[] = {3, 4, 6, 7, 10};
    int value = 5;

    printf("下标为： %d\n", binary_search(a, n, value));
}

int main() {
    test_case_1();
}