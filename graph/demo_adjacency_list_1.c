#include <stdio.h>
#include <stdlib.h>

#define MaxVertexNum 100

typedef char VertexType;
typedef struct node {   // 边表节点
    int adjvex;
    struct node* next;
} EdgeNode;

typedef struct {
    VertexType vertex;
    EdgeNode* firstedge;
} VertexNode;

typedef VertexNode AdjList[MaxVertexNum];

typedef struct {
    AdjList adjlist;
    int n, e;
} ALGraph;

void create(ALGraph*);

/*
读入顶点数和边数(顶点数,边数):4,5
建立顶点表:0
建立顶点表:1
建立顶点表:2
建立顶点表:3
建立边表
读入(vi, vj)的顶点对序号:0,1
读入(vi, vj)的顶点对序号:0,2
读入(vi, vj)的顶点对序号:0,3
读入(vi, vj)的顶点对序号:1,2
读入(vi, vj)的顶点对序号:1,3
 */
int main() {

    ALGraph *G = (ALGraph*)malloc(sizeof(ALGraph));
    create(G);

    for (int i = 0; i < G->n; i++) {
        printf("%d->", i);
        while (G->adjlist[i].firstedge != NULL) {
            printf("%d->", G->adjlist[i].firstedge->adjvex);
            G->adjlist[i].firstedge = G->adjlist[i].firstedge->next;
        }

        printf("\n");
    }
}

void create(ALGraph* G) {
    int i, j, k, w, v;
    EdgeNode *s;

    printf("读入顶点数和边数(顶点数,边数):");
    scanf("%d,%d", &G->n, &G->e);

    for (i = 0; i < G->n; i++) {
        fflush(stdin);
        printf("建立顶点表:");
        scanf("%d", &G->adjlist[i].vertex);
        // G->adjlist[i].vertex = getchar();
        G->adjlist[i].firstedge = NULL;
    }

    printf("建立边表\n");
    for (k = 0; k < G->e; k++) {
        printf("读入(vi, vj)的顶点对序号:");
        scanf("%d, %d", &i, &j);
        s = (EdgeNode*)malloc(sizeof(EdgeNode));
        s->adjvex = j;
        s->next = G->adjlist[i].firstedge;  // 插入表头
        G->adjlist[i].firstedge = s;
        s = (EdgeNode*)malloc(sizeof(EdgeNode));
        s->adjvex = i;
        s->next = G->adjlist[j].firstedge;
        G->adjlist[j].firstedge = s;
    }
}

