#include <iostream>
using namespace std;

/**
 * 稳定排序：检测各算法对于所给输入是否有问题输出
 * 稳定输出
 *  1. 是指当出现多张数字相同的牌时，这些牌在输入与输出中的顺序不变
 *  2. 能时常保证稳定输出的排序算法称为稳定排序算法
 */
struct Card {char suit, value;};

void bubble(struct Card A[], int N) {
    for (int i = 0; i < N; i++) {
        for (int j = N - 1; j >= i + 1; j--) {
            if (A[j].value < A[j - 1].value) {
                Card t = A[j];
                A[j] = A[j - 1];
                A[j - 1] = t;
            }
        }
    }
}

void selection(struct Card A[], int N) {
    for(int i = 0; i < N; i++) {
        int minj = i;
        for (int j = i; j < N; j++) {
            if (A[j].value < A[minj].value) minj = j;
        }
        Card t = A[i];
        A[i] = A[minj];
        A[minj] = t;
    }
}

void print(struct Card A[], int N) {
    for (int i = 0; i < N; i++) {
        if (i > 0) cout << " ";
        cout << A[i].suit << A[i].value;
    }
    cout << endl;
}

// 比较冒泡排序和选择排序的结果
bool isStable(struct Card C1[], struct Card C2[], int N) {
    for (int i = 0; i < N; i++) {
        if (C1[i].suit != C2[i].suit) return false;
    }
    return true;
}

/***
 * 5
 * H4 C9 S4 D2 C3
 */
int main() {

    Card C1[100], C2[100];
    int N;
    char ch;

    cin >> N;

    for (int i = 0; i < N; i++) {
        cin >> C1[i].suit >> C1[i].value;
    }

    for (int i = 0; i < N; i++) C2[i] = C1[i];

    bubble(C1, N);
    selection(C2, N);

    print(C1, N);
    cout << "Stable" << endl;
    print(C2, N);

    if (isStable(C1, C2, N)) {
        cout << "Stable" << endl;
    } else {
        cout << "No Stable" << endl;
    }

    return 0;
}