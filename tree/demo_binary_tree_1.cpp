#include <cstdio>
#define MAX 10000
#define NIL -1

struct Node { int parent, left, right; };

Node T[MAX];
int n, D[MAX], H[MAX];

/**
 * 1. 0 0
 * 2. 1 1
 * 3. 2 2
 * 4. 3 2
 * 5. 4 1
 * 6. 5 2
 * 7. 6 3
 * 8. 7 3
 * 9. 8 2
 */
void setDepth(int u, int d) {
    
    if ( u == NIL) return;
    D[u] = d;
    setDepth(T[u].left, d + 1);
    setDepth(T[u].right, d + 1);
}

/***
 * 1. 0 3
 * 2. 1 1
 * 3. 2 0
 * 4. 3 0
 * 5. 4 2
 * 6. 5 1
 * 7. 6 0
 * 8. 7 0
 * 9. 8 0
 */
int setHeight(int u) {

    int h1 = 0, h2 = 0;
    if (T[u].left != NIL)
        h1 = setHeight(T[u].left) + 1;
    if (T[u].right != NIL)
        h2 = setHeight(T[u].right) + 1;
    return H[u] = (h1 > h2 ? h1 : h2);
}

// 返回结点u的兄弟节点
int getSibling(int u) {

    if (T[u].parent == NIL) return NIL;
    if (T[T[u].parent].left != u && T[T[u].parent].left != NIL)
        return T[T[u].parent].left;
    if (T[T[u].parent].right != u && T[T[u].parent].right != NIL)
        return T[T[u].parent].right;
    return NIL;
}

void print(int u) {

    printf("node %d: ", u);
    printf("parent = %d, ", T[u].parent);
    printf("sibling = %d, ", getSibling(u));
    int deg = 0;
    if (T[u].left != NIL) deg++;
    if (T[u].right != NIL) deg++;
    printf("degree = %d, ", deg);
    printf("depth = %d, ", D[u]);
    printf("height = %d, ", H[u]);

    if (T[u].parent == NIL) {
        printf("root \n");
    } else if (T[u].left == NIL && T[u].right == NIL) {
        printf("leaf \n");
    } else {
        printf("internal node \n");
    }
}

/**
 * {
 *  {parent = 0, left = 1, right = 4}, 
 *  {parent = 0, left = 2, right = 3}, 
 *  {parent = 1, left = -1, right = -1}, 
 *  {parent = 1, left = -1, right = -1}, 
 *  {parent = 0, left = 5, right = 8}, 
 *  {parent = 4,left = 6, right = 7}, 
 *  {parent = 5, left = -1, right = -1}, 
 *  {parent = 5, left = -1, right = -1}, 
 *  {parent = 4, left = -1, right = -1}, 
 *  {parent = 0, left = 0, right = 0}
 * }
 */
int main () {

    int v, l, r, root = 0;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &v, &l, &r);
        T[v].left = l;
        T[v].right = r;
        if (l != NIL) T[l].parent = v;
        if (r != NIL) T[r].parent = v;
    }

    for (int i = 0; i < n; i++) if (T[i].parent == NIL) root = i;

    setDepth(root, 0);
    setHeight(root);

    for (int i = 0; i < n; i++) print(i);

    return 0;
}