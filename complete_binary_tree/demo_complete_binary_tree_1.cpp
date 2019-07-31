#include <iostream>
using namespace std;
#define MAX 100000

int parent (int i) { return i / 2; }
int left (int i) { return 2 * i; }
int right (int i ) { return 2 * i + 1; }

int main ()
{
    int H, i,A[MAX + 1];

    cin >> H;
    for (i = 1; i < H; i++) cin >> A[i];

    for (i = 1; i < H; i++) {
        cout << "node " << i << ": key = " << A[i], ", ";
        if (parent(i) >= 1 ) cout << "parent key= " << A[parent(i)] << ", ";
        if (left(i) <= H) cout << "left key= " << A[left(i)] << ", ";
        if (right(i) <= H) cout << "right key= " << A[right(i)] << ", ";
        cout << endl;
    }
    return 0;
}