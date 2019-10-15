#include <stdio.h>

#define Max_ 10             // 数组个数
#define RADIX_10 10         // 整型排序
#define KEYNUM_31 10        // 关键字个数，这里为整型位数

// 打印结果
void show(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 找到num的从低到高的第pos位数据
int getNumInPos(int num, int pos) {
    int temp = 1;
    int i;
    for (i = 0; i < pos - 1; i++) {
        temp *= 10;
    }
    return (num / temp) % 10;
}

// 基数排序 pDataArray 无序排序：iDataNum 为无序数据个数
void radixSort(int *pDataArray, int iDataNum) {
    int *radixArrays[RADIX_10];     // 分别为 0 ～ 9的序列空间
    int i, j, pos, k;

    for (i = 0; i < 10; i++) {
        radixArrays[i] = (int *) malloc(sizeof(int) * (iDataNum + 1));
        radixArrays[i][0] = 0;      // index 为0处记录这组数据的个数
    }

    for (pos = 1; pos <= KEYNUM_31; pos++) {    // 从个位开始到31位
        for (i = 0; i < iDataNum; i++) {        // 分配过程
            int num = getNumInPos(pDataArray[i], pos);
            int index = ++radixArrays[num][0];
            radixArrays[num][index] = pDataArray[i];
        }

        for (i = 0, j = 0; i < RADIX_10; i++) { // 收集
            for (k = 1; k <= radixArrays[i][0]; k++) {
                pDataArray[j++] = radixArrays[i][k];
            }
            radixArrays[i][0] = 0;              // 复位
        }
    }
}


void radxSortTestCase1() {
    //测试数据
    int arr_test[Max_] = { 8, 4, 2, 3, 5, 1, 6, 9, 0, 7 };
    //排序前数组序列
    show(arr_test, Max_);
    radixSort(arr_test, Max_);
    //排序后数组序列
    show( arr_test, Max_ );
}

void radxSortTestCase2() {
    //测试数据
    int arr_test[Max_] = {123, 12341, 12341, 124, 236, 128, 1112313129, 98, 9, 8989};
    //排序前数组序列
    show(arr_test, Max_);
    radixSort(arr_test, Max_);
    //排序后数组序列
    show( arr_test, Max_ );
}

int main() {
    radxSortTestCase2();
    return 0;
}