#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 棋盘格子的范围，以及皇后数量
#define MAX_NUM 8
// 二维数组作为棋盘
int chessBoard[MAX_NUM][MAX_NUM] = {0}; 

// 检查落点是否符合规则
bool check(int x, int y) {
    int i;
    for (i = 0; i < y; i++) {
        // 检查纵向
        if (chessBoard[x][i] == 1) {
            return false;
        }

        // 检查左侧斜向
        if (x - 1 - i >= 0 && chessBoard[x - 1 - i][y - 1 - i] == 1) {
            return false;
        }

        // 检查右侧斜向
        if (x + 1 + i < MAX_NUM && chessBoard[x + 1 + i][y - 1 - i] == 1) {
            return false;
        }
    }
    return true;
}

bool settleQueue(int y) {
    int i, x;
    // 行数超过8，说明已经找出答案
    if (y == MAX_NUM) return true;

    // 遍历当前行，逐一格子验证
    for (i = 0; i < MAX_NUM; i++) {
        // 为当前行清零，以免在回溯的时候出现脏数据
        for (x = 0; x < MAX_NUM; x++) {
            chessBoard[x][y] = 0;
        }
        // 检查是否符合规则，如果符合，更改元素值并进一步递归
        if (check(i, y)) {
            chessBoard[i][y] = 1;
            // 递归如果返回true，说明下层已找到解法，无需继续循环
            if (settleQueue(y + 1)) {
                return true;
            }
        }
    }
    return false;
}

// 打印棋盘当前值
void printChessBoard() {
    int i, j;
    for (j = 0; j < MAX_NUM; j++) {
        for (i = 0; i < MAX_NUM; i++) {
            printf("%d", chessBoard[i][j]);
        }
        printf("\n");
    }
}

int main() {

    settleQueue(0);
    printChessBoard();
    return 0;
}