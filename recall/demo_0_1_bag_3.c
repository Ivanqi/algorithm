#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int maxW = 0;                   // 结果放到maxW中
int weight[] = {2, 2, 4, 6, 3};   // 物品重量
int n = 5;                      // 物品个数
int w = 9;                      // 背包承受的最大重量

bool mem[5][10] = {0};          // 备忘录，默认值false

void f(int i, int cw) {
    if (cw == w || i == n) {    // cw == w表示装满了， i == n表示物品都考察完了
        if (cw > maxW) maxW = cw;
        return;
    }
    printf("i%d, cw%d\n", i, cw);
    if (mem[i][cw]) return;     // 重复状态
    mem[i][cw] = true;          // 记录(i, cw)这个状态
    f(i + 1, cw);               // 选择不装第i个物品
    if (cw + weight[i] <= w) {
        f(i + 1, weight[i]);    // 选择装第i个物品
    }
}

int main() {
    f(0, 0);
    printf("maxW:%d\n", maxW);
}
