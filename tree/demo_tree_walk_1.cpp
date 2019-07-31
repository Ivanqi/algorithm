#include <stdio.h>
#define MAX 10000
#define NIL -1

struct Node { int p, l, r; };
struct Node T[MAX];
int n;


/**
 * 前序遍历
 */
void preParse(int u) {

    if (u == NIL) return;
    printf(" %d", u);
    preParse(T[u].l);
    preParse(T[u].r);
}

/**
 * 中序遍历
 */
void inParse(int u) {
    
    if (u == NIL) return;
    inParse(T[u].l);
    printf(" %d", u);
    inParse(T[u].r);
}

/**
 * 后序遍历
 */
void postParse(int u) {

    if (u == NIL) return;
    postParse(T[u].l);
    postParse(T[u].r);
    printf(" %d", u);
}

/**
 * {
 *  {p = -1, l = 1, r = 4}, 
 *  {p = 0, l = 2, r = 3}, 
 *  {p = 1, l = -1, r = -1}, 
 *  {p = 1, l = -1, r = -1}, 
 *  {p = 0, l = 5, r = 8}, 
 *  {p = 4, l = 6, r = 7}, 
 *  {p = 5, l = -1, r = -1}, 
 *  {p = 5, l = -1, r = -1}, 
 *  {p = 4, l = -1, r = -1}, 
 *  {p = 0, l = 0, r = 0} 
 * }
 */
int main () {
    
    int i, v, l, r, root;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        T[i].p = NIL;
    }

    for (i = 0; i < n ; i++) {
        scanf("%d %d %d", &v, &l, &r);
        T[v].l = l;
        T[v].r = r;
        if ( l != NIL) T[l].p = v;
        if ( r != NIL) T[r].p = v;
    }

    for (i = 0; i < n; i++) if ( T[i].p == NIL) root = i;

    printf("Preorder\n");
    preParse(root);
    printf("\n");
    printf("Inorder\n");
    inParse(root);
    printf("\n");
    printf("Postorder\n");
    postParse(root);
    printf("\n");

    return 0;
}