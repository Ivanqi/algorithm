#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int* FindTopk(int* arr, int n, int* ret, int m) {
    priority_queue<int, vector<int>, greater<int>> q(arr, arr + m);

    int i = m;

    for (; i < n; i++) {
        if (arr[i] > q.top()) {
            q.pop();
            q.push(arr[i]);
        }
    }

    int j = 0;
    while (!q.empty()) {
        ret[j++] = q.top();
        q.pop();
    }
}

int main() {
    int arr[] = {9,4,5,2,5,1,7,3,1,8};
    int ret[5];

    FindTopk(arr, 10, ret, 5);

    for(int i = 0;i<5;i++)
        cout << ret[i] <<" ";
    return 0;
}