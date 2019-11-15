#include <iostream>
using namespace std;
#define MAX 2000000

int H, A[MAX + 1];

void maxHeadpify(int i) {
    int l, r, largest;
    l = 2 * i + 1;
    r = 2 * i + 2;

    // 从左子节点，自身，右子节点选出值最大的节点
    if (l <= H && A[l] > A[i]) largest = l;
    else largest = i;

    if (r <= H && A[r] > A[largest]) largest = r;

    if (largest != i) {
        swap(A[i], A[largest]);
        maxHeadpify(largest);
    }
}

/**
 * 10
 * 4 1 3 2 16 9 10 14 8 7
 */
int main() {

    cin >> H;

    for (int i = 0; i < H; i++) cin >> A[i];

    for (int i = (H - 2) / 2; i >= 0; i--) maxHeadpify(i);

    for (int i = 0; i < H; i++) {
        cout << " " << A[i];
    }
    cout << endl;

    return 0;
}