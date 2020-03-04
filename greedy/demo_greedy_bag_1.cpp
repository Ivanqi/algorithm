#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct bag {
    int weight; // 重量
    int value;  // 总价值
    float bi;   // 单位重量的价值
    float rate; // 使用率：1表示完整代入，小于1代表被分割后放入
} bags[50];

bool compare (const bag &bag1, const bag &bag2) {
    return bag1.bi > bag2.bi;
}

void greedy(vector<int>& weight, vector<int>& price, float capacity, int num) {

    int sum = 0;
    int j = 0;
    for (int i = 0; i < num; i++) {
        bags[i].weight = weight[i];
        bags[i].value = price[i];
        bags[i].bi = (float) bags[i].value / bags[i].weight;
        bags[i].rate = 0;
    }

    sort(bags, bags + num, compare); 

    for (j = 0; j < num; j++) {
        // cout << "weight:" << bags[j].weight << " | value: " << bags[j].value << " | bi:" << bags[j].bi << endl;
        if (bags[j].weight <= capacity) {
            bags[j].rate = 1;
            sum += bags[j].weight;
            capacity -= bags[j].weight;
            cout << "重: " << bags[j].weight << ", 价值: " << bags[j].value << " 的物品被放入了背包 "  << ", 放入比例: " << bags[j].rate << endl;  
        } else break;
    }
    cout << "背包已容纳数量(j): " << j << " | 背包剩余容量(capacity): " << capacity << endl;

    if (j < num) {
        bags[j].rate = capacity / bags[j].weight;
        sum += bags[j].rate * bags[j].weight;
        cout << "重：" << bags[j].weight << " 价值：" << bags[j].value << " 被放入了背包" << ", 放入比例:" << bags[j].rate <<endl;
    }

}

void test_case_1 () {
    vector<int> weight {35, 30, 60, 50, 40, 10, 25};
    vector<int> price {10, 40, 30, 50, 35, 40, 30};

    float capacity = 150;
    int num = 7;

    greedy(weight, price, capacity, num);
}

void test_case_2() {
    vector<int> weight {100, 30, 60, 20, 50};
    vector<int> price {100, 90, 120, 80, 75};

    float capacity = 100;
    int num = 5;

    greedy(weight, price, capacity, num);
}

int main () {

    test_case_2();
    return 0;
}