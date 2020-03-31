#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 八皇后：在8 * 8国际象棋盘上，有8个皇后，每个皇后占一格
// 要求皇后之间不会出现相互“攻击”的现象，即不能出现两个皇后处在同一行，同一列或同一对角线
// 全局或成员变量，下标表示行，值表示queen存储在那一列
int result[8];
int soluation = 0;

void printQueens(int *result) { // 打印出一个二维矩阵
    int row, column;
    for (row = 0; row < 8; ++row) {
        for (column = 0; column < 8; ++column) {
            if (result[row] == column) {
                printf("Q ");
            } else {
                printf("* ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

bool isOk(int row, int column) { // 判断row行column列放置是否合适
    int leftUp = column - 1, rightUp = column + 1;
    // printf("isOK, row:%d, column:%d, leftUp:%d, rightUp:%d\n", row, column, leftUp, rightUp);
    int i;
    for (i = row - 1; i >= 0; --i) {            // 逐行往上考察每一行
        if (result[i] == column) return false;  // 第i行的column列有棋子吗？
        if (leftUp >= 0) {                      // 考察左上对角线：第i行leftup列有棋子吗？
            if (result[i] == leftUp) return false;
        }

        if (rightUp < 8) {                      // 考察右上对角线，第i行rightup列有棋子吗？
            if (result[i] == rightUp) return false;
        }
        --leftUp;
        ++rightUp;
    }
    // printf("isOK2, row:%d, column:%d, leftUp:%d, rightUp:%d\n", row, column, leftUp, rightUp);
    return true;
}

void cal8Queens(int row) {
    if (row == 8) {                             // 8个棋子都放置好了，打印结果
        soluation++;
        printQueens(result);
        return;                                 // 8行棋子都放好了，已经没法再往下递归了，所以就return
    }

    int column;
    for (column = 0; column < 8; ++column) {    // 每一行都有8种方法
        if (isOk(row, column)) {                // 有些方法不满足要求
            result[row] = column;               // 第row行的棋子放到了column行
            cal8Queens(row + 1);                // 考察下一行
        }
    }
}

/**
 * 1. 为什么会生成92中解法?
 *   1. 因为 cal8Queens， 没有设置停止状态。每次循环cal8Queens，没有退出状态，意味着要找出所有可能
 *   2. 求一种正确的解法是，深度优先遍历(DFS)
 *   3. 找出全部正确的解法是, 广度优先遍历(BFS)
 * 2. 为什么不需要清除脏数据?
 *   1. 因为使用了一维数组。解法不对，需要回溯，到下一个column的时候，row还是一样的。直接按row覆盖数据就可以了
 */

int main() {

    cal8Queens(0);
    printf("共有%d种解法\n", soluation);
    return 0;
}