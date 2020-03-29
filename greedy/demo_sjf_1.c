#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 10    // 允许最大进程个数
#define M 100   // 进程名称长度
int n;          // 进程个数
char name[N][M];    // 进程名
int Arival[N] = {0};    // 到达时间
int Go[N] = {0};        // 运行时间
int Start[N] = {0};     // 开始时间
int End[N] = {0};       // 结束时间
int Timer[N] = {0};     // 周转时间
float DTimer[N] = {0};  // 带权周转时间
int Check[N] = {0};     // 判断作业是否完成，完成值为1

// 输入函数
void input() {
    int i;
    printf("进程的个数:");
    scanf("%d",&n);
    for(i = 0; i < n; i++){
        printf("进程名：");
        scanf("%s", &name[i]);
        printf("第%d个进程的到达时间:", i + 1);
        scanf("%d",Arival + i);
        printf("第%d个进程的运行时间:", i + 1);
        scanf("%d", Go + i);
    }
}

/**
 * 选出先到达的作业
 * a[] 到达时间
 * n 进程个数
 */
int Select0(int a[], int n) {
    int i = 0;
    int k;
    int j;
    for (k = 0; k < n; k++) {
        if (Check[k] == 0) {
            i = k;
            break;
        }
    }

    for (j = 0; j < n; j++) {
        if (a[i] > a[j] && Check[j] == 0) {
            i = j;
        }
    }

    Check[i] = 1;
    return i;
}

// 先来先服务调度算法
void fcfs() {
    int k = 0;          // 每次选出即将服务的进程
    int l = 0;          // 本次服务的进程
    int Atimer = 0;     // 周转时间之和
    float timer = 0;    // 带权周转时间只和
    int m;

    // 每次开始之前Check数组要全部置为0
    memset(Check, 0, sizeof(Check));
    k = Select0(Arival, n);
    Start[k] = Arival[k];
    End[k] = Start[k] + Go[k];
    Timer[k] = End[k] - Arival[k];
    DTimer[k] = (float)Timer[k] / Go[k];

    printf("作业  提交时间  运行时间  开始时间  结束时间  周转时间  带权周转时间\n");
    for (m = 0; m < n; m++) {
        l = k;
        k = Select0(Arival, n);
        Start[k] = End[l];
        End[k] = Start[k] + Go[k];
        Timer[k] = End[k] - Arival[k];
        DTimer[k] = (float) Timer[k] / Go[k];
        Atimer = Timer[l] + Atimer;
        timer = timer + DTimer[l];
        printf(" %s     %2d        %2d         %2d        %2d        %2d         %.2f\n",name[l],Arival[l],Go[l],Start[l],End[l],Timer[l],DTimer[l]);
    }
    printf("平均周转时间:%.2f\n", (float)Atimer / n);
    printf("平均带权周转时间:%.2f\n", (float)timer / n);
}

/**
 * 选出短作业
 * a[] 运行时间
 * n 进程个数
 * local 当前时间
 */
int Select1(int a[], int n, int local) {
    int i = 0;
    int k;
    int j;
    for (k = 0; k < n; k++) {
        if (Check[k] == 0) {
            i = k;
            break;
        }
    }

    for(j = 0; j < n; j++) {
        if (a[i] > a[j] && Check[j] == 0 && Arival[j] <= local) {
            i = j;
        }
    }

    Check[i] = 1;
    return i;
}

// 短作业优先调度算法
void sjf() {
    int k = 0;      // 每次选出即将服务的进程
    int l = 0;      // 本次服务的进程
    int Atimer = 0; // 周转时间之和
    int timer = 0;  // 带权周转时间之和
    int localtime=0;	//当前时间 
    int m;

    // 每次开始之前Check数组要全部置为0
    memset(Check, 0, sizeof(Check));
    Start[k] = Arival[k];
    End[k] = Start[k] + Go[k];
    Timer[k] = End[k] - Arival[k];
    DTimer[k] = (float)Timer[k] / Go[k];
    localtime = End[k];
    Check[k] = 1;
    printf("作业  提交时间  运行时间  开始时间  结束时间  周转时间  带权周转时间\n");

    for (m = 0; m < n; m++) {
        l = k;
        k = Select1(Go, n, localtime);
        Start[k] = End[l];
        End[k] = Start[k] + Go[k];
        Timer[k] = End[k] - Arival[k];
        DTimer[k] = (float)Timer[k] / Go[k];
        localtime = End[k];
        Atimer = Timer[l] + Atimer;
        timer = timer + DTimer[l];
        printf(" %s     %2d        %2d         %2d        %2d        %2d         %.2f\n",name[l],Arival[l],Go[l],Start[l],End[l],Timer[l],DTimer[l]);
    }
    printf("平均周转时间:%.2f\n", (float) Atimer / n);
    printf("平均带权周转时间:%.2f\n", (float) timer / n);
}

void menu() {
    int choice;
    while (1) {
        printf("*******请选择调度算法*******\n\t1、先来先服务\n\t2、短作业优先\n\t0、退出\n请输入：");
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            fcfs();
        } else if (choice == 2) {
            sjf();
        } else {
            printf("输入有误!\n");
        }
    }
}

int main() {
    input();
    menu();

    return 0;
}