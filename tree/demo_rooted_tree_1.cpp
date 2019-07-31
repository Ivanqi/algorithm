#include <iostream>

using namespace std;
#define MAX 100005
#define NIL -1

struct Node { int p, l, r; };

Node T[MAX];
int n, D[MAX];

void print(int u) {
    int i, c;
    cout << "node " << u << ": ";
    cout << "parent = " << T[u].p << ", ";
    cout << "depth = " << D[u] << ", ";

    if (T[u].p == NIL) cout << "root, " ;
    else if (T[u].l == NIL) cout << "leaf, ";
    else cout << "internal node, ";

    cout << "[";

    for (i = 0, c = T[u].l; c != NIL;i++, c = T[c].r) {
        if (i) cout << ", ";
        cout << c;
    }
    cout << "]" << endl;
}

// 递归地求深度
/*
    1. 0 0
    2. 1 1
    3. 2 2
    4. 3 2
    5. 4 1
    6. 5 2
    7. 6 2
    8. 7 2
    9. 8 3
    10. 9 3
    11. 10 1
    12. 11 2
    13. 12 2
    14. 0 0
 */
int rec(int u, int p) {
    D[u] = p;
    if (T[u].r != NIL) rec(T[u].r, p);          // 右侧兄弟设置为相同深度
    if (T[u].l != NIL) rec(T[u].l, p + 1);      // 最左侧子结点的深度设置为自己的深度 + 1
}
 
 /**
  * 
  * {
        {p = -1, l = 1, r = -1},
        {p = 0, l = 2, r = 4},
        {p = 1,l = -1, r = 3},
        {p = 1,l = -1, r = -1},
        {p = 0, l = 5, r = 10},
        {p = 4, l = -1, r = 6},
        {p = 4, l = -1, r = 7},
        {p = 4, l = 8, r = -1},
        {p = 7, l = -1, r = 9},
        {p = 7, l = -1, r = -1},
        {p = 0, l = 11, r = -1},
        {p = 10, l = -1, r = 12},
        {p = 10, l = -1, r = 12},
        {p = 10, l = -1, r = -1},
        {p = 0, l = 0, r = 0}
  * }

  */
int main () {

    int i, j, d, v, c, l, r;
    cin >> n;
    for (i = 0; i < n; i++) T[i].p = T[i].l = T[i].r = NIL;

    for (i = 0; i < n; i++) {
        cin >> v >> d;
        for (j = 0; j < d; j++) {
            cin >> c;
            if (j == 0) T[v].l = c;
            else T[l].r = c;
            l = c;
            T[c].p = v;
        }
    }
    
    for (i = 0; i < n; i++) {
        if (T[i].p == NIL) r = i;
    }

    rec(r, 0);
    
    for (i = 0; i < n; i++) print(i);

    return 0;
    
}