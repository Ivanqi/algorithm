#include <stdio.h>
#include <stdlib.h>

// cw表示当前已经装进去的物品的重量和；i表示考察到哪个物品了；
// 假设背包可承受重量100，物品个数10，物品重量存储在数组a中，那可以这样调用函数：f(0, 0)
int nCurWeight = 0;     // 当前存储背包中物品总重量
int BagWeightMax = 16; // 背包能承受的总重量
const int nTotalItemCount = 3; // 道具总个数
int items[10] = {1, 5, 2};  // 每个道具重量

void f(int i, int cw) {
    if (cw == BagWeightMax || i == nTotalItemCount) {   // cw==BagWeightMax表示装满了;i==n表示已经考察完所有的物品
        if (cw > nCurWeight) {
            nCurWeight = cw;
        }
        printf("nCurWeight:%d\n", nCurWeight);
        return;
    }
    printf("out:%d\n", i);
    f(i + 1, cw);   // 当前物品不装进背包
    if (cw + items[i] <= BagWeightMax) {
        // 已经超过背包可以承受的重量的时候，就不要再装了，剪枝法
        printf("inner:%d\n", i);        
        f(i + 1, cw + items[i]);    // 当前物品装进背包
    }
}


int main() {
    f(0, 0);
}