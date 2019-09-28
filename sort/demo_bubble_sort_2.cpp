#include <iostream>

using namespace std;

int* bubbleSort(int A[], int N) {
    bool flag = 1;
    for (int i = 0; flag; ++i) {
        flag = 0;
        for (int j = 0; j < N - i - 1; ++j) {
            if (A[j] > A[j + 1]) {
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
                flag = 1;
            }
        }
    }
    return A;
}

int main() {

    int arr[] = {4, 5, 6, 3, 2, 1};
    int n = 6;

    int *res = bubbleSort(arr, n);
    for (int i = 0; i < n; i++) {
        cout << "val: " << res[i] << endl;
    }
    return 0;
}