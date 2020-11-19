#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

/**
 * 判断无向图是否是树
 * https://blog.csdn.net/cbwem/article/details/79080529
 * https://blog.csdn.net/shuiyixin/article/details/84000379
 */

// 图的邻接表类型定义
typedef char VertexType[4];
typedef char InfoPtr;
typedef int VRType;

#define MaxSize 50  // 最大顶点个数
typedef enum {DG, DN, UG, UN} GraphKind;    // 图的类型：有向图，有向网，无向图和无向网

typedef struct ArcNode {                    // 边表结点的类型定义
    int adjvex;                             // 弧指向的顶点的位置
    InfoPtr *info;                          // 与弧相关的信息
    struct ArcNode *nextarc;                // 指示下一个与该顶点相邻的顶点
} ArcNode;

typedef struct VNode{                       // 表头结点的类型定义
    VertexType data;                        // 用于存储顶点
    ArcNode *firstarc;                      // 指示第一个与该顶点邻接的顶点
} VNode, AdjList[MaxSize];

typedef struct  {                           // 图的类型定义
    AdjList vertex;
    int vexnum, arcnum;                     // 图的顶点数目与弧的数目
    GraphKind kind;                         // 图的类型
} AdjGraph;

// 函数声明
int LocateVertex(AdjGraph G, VertexType v);
void CreateGraph(AdjGraph *G);
void DisplayGraph(AdjGraph G);
void DestroyGraph(AdjGraph *G);
int IsTree(AdjGraph *G);
void DFS(AdjGraph *G, int v, int *vNum, int *eNum);
int visited[MaxSize];


/**
 *  输入图的顶点数，边数(逗号分隔): 5,4
    输入5个顶点的值:
    v1 v2 v3 v4 v5
    输入弧尾和弧头(以空格作为间隔 ):
    v1 v2
    v1 v3
    v2 v4
    v2 v5
 */
int main() {
    
    AdjGraph G;
    printf("采用邻接矩阵创建无向图G: \n");
    CreateGraph(&G);
    printf("输出无向图G: ");
    DisplayGraph(G);

    if (IsTree(&G)) {
        printf("无向图G是一棵树!\n");
    } else {
        printf("无向图G不是一棵树!\n");
    }

    DestroyGraph(&G);

    return 0;
}

int IsTree(AdjGraph *G) {
    int vNum = 0, eNum = 0, i;
    for (i = 0; i < G->vexnum; i++) {
        visited[i] = 0;
    }

    DFS(G, 0, &vNum, &eNum);
    if (vNum == G->vexnum && eNum == 2 * (G->vexnum - 1)) {
        return 1;
    } else {
        return 0;
    }
}

void DFS(AdjGraph *G, int v, int *vNum, int *eNum) {
    ArcNode *p;
    visited[v] = 1;
    (*vNum)++;

    p = G->vertex[v].firstarc;

    while (p != NULL) {
        (*eNum)++;
        if (visited[p->adjvex] == 0) {
            DFS(G, p->adjvex, vNum, eNum);
        }
        p = p->nextarc;
    }
}

// 采用邻接表存储结构，创建无向图G
void CreateGraph(AdjGraph *G) {
    int i, j, k;
    VertexType v1, v2;          // 定义两个顶点v1和v2
    ArcNode *p;

    printf("输入图的顶点数，边数(逗号分隔): ");
    scanf("%d, %d", &(*G).vexnum, &(*G).arcnum);
    printf("输入%d个顶点的值:\n", G->vexnum);

    for (i = 0; i < G->vexnum; i++) {   // 将顶点存储在表头结点中
        scanf("%s", G->vertex[i].data);
        G->vertex[i].firstarc = NULL;   // 将相关的顶点置为空
    }

    printf("输入弧尾和弧头(以空格作为间隔 ): \n");
    for (k = 0; k < G->arcnum; k++) {
        scanf("%s%s", v1, v2);

        i = LocateVertex(*G, v1);
        j = LocateVertex(*G, v2);

        // j为入边i为出边创建邻接表
        p = (ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex = j;
        p->info = NULL;
        p->nextarc = G->vertex[i].firstarc;
        G->vertex[i].firstarc = p;
        
        // i为入边，j为出边。创建邻接表
        p = (ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex = i;
        p->info = NULL;
        p->nextarc = G->vertex[j].firstarc;
        G->vertex[j].firstarc = p;
    }

    (*G).kind = UG;
}

// 返回图中顶点对应的位置
int LocateVertex(AdjGraph G,VertexType v)
{ 
    int i;
    for (i = 0; i < G.vexnum; i++) {
        if(strcmp(G.vertex[i].data,v)==0) {
            return i;
        }
    }
    return -1;
}

// 销毁无向图G
void DestroyGraph(AdjGraph *G) {
    int i;
    ArcNode *p, *q;
    for (i = 0; i < (*G).vexnum;++i) {      // 释放图中的边表结点
        p = G->vertex[i].firstarc;          // p 指向边表的第一个结点
        if (p != NULL) {                    // 如果边表不为空，则释放边表的结点
            q = p->nextarc;
            free(p);
            p = q;
        }
    }
    (*G).vexnum = 0;                        // 将顶点数置为0
    (*G).arcnum = 0;                        // 将边的数目置为0
}

// 图的邻接表存储结构的输出
void DisplayGraph(AdjGraph G) {
    int i;
    ArcNode *p;
    printf("%d个顶点: \n", G.vexnum);

    for (i = 0; i < G.vexnum; i++) {
        printf("%s  ", G.vertex[i].data);
    }

    printf("\n%d条边:\n", 2 * G.arcnum);

    for (i = 0; i < G.vexnum; i++) {
        p = G.vertex[i].firstarc;       // 将p指向边表的第一个结点
        while (p) {                     // 输出无向图的所有边
            printf("%s -> %s ", G.vertex[i].data, G.vertex[p->adjvex].data);
            p = p->nextarc;
        }
        printf("\n");
    }
}