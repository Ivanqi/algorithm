#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * partition 会把整体分成两个部分
 * 更具体的，会用数组arr中的一个元素(默认是第一个元素 t= arr[low]) 为划分依据，将数据arr[low, hight]划分成左右两个子数组
 *  1. 左半部分，都比t大
 *  2. 右半部分，都比t小
 *  3. 中间位置i是划分元素
 */
int partition(int *arr, int p, int r) {
    int i, j;
    i = j = p;
    for (; j < r; j++) {
        if (arr[j] >= arr[r]) {
            if (i != j) {
                swap(arr + i, arr + j);
            }
            i++;
        }
    }
    swap(arr + i, arr + j);
    return i;
}

/**
 * 随机选择算法(randomized_select)
 * partition 和 TopK问题有什么关系？
 *  1. TopK是希望求出arr[1, n]中最大的k个数，那如果找到了第k大的数，做一次partition，不就一次性找到最大的k个数了吗？
 *  2. 再回过头来看看第一次partition，划分之后(i = partition(arr, 1, n))
 *     1. 如果i 大于k, 则说明arr[i]左边的元素都大于k，于是只递归arr[1, i - 1]里第k大元素即可
 *     3. 如果i小于k，则说明第k大的元素在arr[i]的右边，于是只递归arr[i + 1, n]里第 k - i 大的元素即可
 */
int RS(int *arr, int low, int high, int k) {
    if (low == high) return;
    int i = partition(arr, low, high);
    int temp = i - low;                     // 数组前半部分元素个数

    if (temp >= k) {
        RS(arr, low, i - 1, k);             // 求前半部分第k大
    } else {
        RS(arr, i + 1, high, k - i);        // 求后半部分第k-i大
    }
}

/**
 * 分治法，大问题分解为小问题，小问题都要递归各个分支，例如：快速排序[ O(n*lg(n)) ]
 * 减治法，大问题分解为小问题，小问题只要递归一个分支，例如：二分查找[ O(lg(n)) ]，随机选择 [ O(n) ]
 */
test_topk_case_1(int topK) {
    int arr[10] = {5, 8, 9, 23, 67, 1, 3, 7, 31, 56};
    int i;
	RS(arr, 0, 9, topK);
	for (i = 0; i < topK; i++) {
        printf("%d \n", arr[i]);
    }
}

int main() {
    int topK = 3;
    test_topk_case_1(topK);
}