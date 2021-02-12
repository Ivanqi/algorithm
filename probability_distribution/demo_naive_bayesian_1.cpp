#include <iostream>
#include <set>
#include <vector>
using namespace std;

// 参考资料: https://blog.csdn.net/IDMer/article/details/48809677
// 定义训练数据
#define M 3
#define N 15

/**
 * 为了计算简单，对A2 = {S, M, L}，令S = 1, M = 2, L = 3;
 * Y = {1, -1}, 令为 Y = {1, 2}
 */
int A[M][N] = {
    {1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 3 , 3 , 3 , 3 , 3},    // X(1)
    {1 , 2 , 2 , 1 , 1 , 1 , 2 , 2 , 3 , 3 , 3 , 2 , 2 , 3 , 3},    // X(2)
    {2 , 2 , 1 , 1 , 2 , 2 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 2}     // Y
};

/**
 * 前提数据
 *  P(Y = 1) = 9 /15 P(Y = 2) = 6 / 15
 * 
 *  P(X(1) = 1 | Y = 1) = 2 / 9, P(X(1) = 2 | Y = 1) = 3 / 9, P(X(1) = 3 | Y = 1) = 4 / 9
 *  P(X(2) = 1, Y = 1) = 1/ 9, (X(2) = 2 | Y = 1) = 4 / 9, P(X(2) = 3 | Y = 1) = 4 / 9
 
 *  P(X(1) = 1, Y = 2) = 3 / 6, P(X(1) = 2 | Y = 2) = 2 / 6, P(X(1) = 3 | Y = 2) = 1 / 6
 *  P(X(2) = 1, Y = 2) = 3 / 6, P(X(2) = 2 | Y = 2) = 2 / 6, P(X(3) = 3 | Y = 2) = 1 / 6
 * 
 * P(Y = 1) * P(X(1) = 2 | Y = 1) * P(X(2) = 1 | Y = 1) = (9 / 15) * (3 / 9) * (1 / 8) = 1 / 45
 * 
 * p(Y = 2) * P(X(1) = 2 | Y = 2) * P(X(2) = 1 | Y = 2) = (6 / 15) * (2 / 6) * (3 / 6) = 1 / 15
 * 
 */

struct Px1 {
    int x1;
    int y;
    double p_x1y;
};

struct Px2 {
    int x2;
    int y;
    double p_x2y;
};

double p[2];
Px1 px1[6];
Px2 px2[6];

// 计算先验概率和条件概率
void calP() {
    // 计算先验
    // double p[2]
    int i, j, k;
    multiset<int>  m_x1, m_x2, m_y; // 多重集容器
    multiset<int>::iterator pos1;

    set<int> x1, x2, y; // 集合容器
    set<int>::iterator pos2, pos3;

    // 运用多重集容器和集合容器
    for (i = 0; i < N; i++) {
        m_x1.insert(A[0][i]);
        m_x2.insert(A[1][i]);
        m_y.insert(A[2][i]);

        x1.insert(A[0][i]);
        x2.insert(A[1][i]);
        y.insert(A[2][i]);
    }

    p[0] = m_y.count(1) / (double) N;    // p(Y = 1)
    p[1] = m_y.count(2) / (double) N;    // p(Y = 2)
    cout << endl << "************先验***********" << endl;
    cout << "p(Y = 1) = " << p[0] << endl;
    cout << "p(Y = 2) =" << p[1] << endl;

    // 计算条件概率
    cout << endl;
    cout << "*********条件概率********" << endl;
    // int px1_num = 3 * 2
    // int px2_num = 3 * 2

    j = 0;
    for (pos2 = y.begin(); pos2 != y.end(); pos2++) {
        for (pos3 = x1.begin(); pos3 != x1.end(); pos3++) {
            px1[j].y = *pos2;
            px1[j].x1 = *pos3;

            int count_x1y = 0;
            for (k = 0; k < N; k++) {
                if (A[0][k] == px1[j].x1 && A[2][k] == px1[j].y) {
                    count_x1y++;
                }
            }
            
            px1[j].p_x1y = count_x1y / (double)m_y.count(px1[j].y); // 计算p(x1 | y) 的概率
            j++;
        }
    }

    cout << "p(x1 | y): " << endl;
    for (j = 0; j < 6; j++) {
        cout << px1[j].x1 << " " << px1[j].y << " " << px1[j].p_x1y << endl;
    }

    j = 0;
    for (pos2 = y.begin(); pos2 != y.end(); pos2++) {
        for (pos3 = x2.begin(); pos3 != x2.end(); pos3++) {
            px2[j].y = *pos2;
            px2[j].x2 = *pos3;

            int count_x2y = 0;
            for (k = 0; k < N; k++) {
                if (A[1][k] == px2[j].x2 && A[2][k] == px2[j].y) {
                    count_x2y++;
                }
            }

            px2[j].p_x2y = count_x2y / (double)m_y.count(px2[j].y); // 计算p(x2 | y) 的概率
            j++;
        }
    }

    cout << "p(x2 | y): " << endl;
    for (j = 0; j < 6; j++) {
        cout << px2[j].x2 << " " << px2[j].y << " " << px2[j].p_x2y << endl;
    }
}

int main() {

    int i = 0, j = 0;
    // 输出训练数据
    cout << "***********训练数据************" << endl;

    for (i = 0; i < M; i++) {
        if (i == 0) {
            cout << "X1: ";
        } else if (i == 1) {
            cout << "X2: ";
        } else if (i == 2) {
            cout << " Y: ";
        }

        for (int j = 0; j < N; j++) {
            cout << " " << A[i][j];
        }
        cout << endl;
    }

    calP(); // 计算先验和条件概率

    int s_x1, s_x2;
    double result[2];
    int class_y = 1;

    cout << "*************预测***************" << endl;
    cout << endl << endl << "Input:";
    cin >> s_x1 >> s_x2;

    for (i = 0; i < 2; i++) {
        double s_px_1, s_px_2;
        for (j = 0; j < 6; j++) {
            if (s_x1 == px1[j].x1 && px1[j].y == class_y) {
                s_px_1 = px1[j].p_x1y;
            }

            if (s_x2 == px2[j].x2 && px2[j].y == class_y) {
                s_px_2 = px2[j].p_x2y;
            }
        }
        cout << "p[i]: " << p[i] << " | s_px_1: " << s_px_1 << " | s_px_2: " << s_px_2 << endl; 
        result[i] = p[i] * s_px_1 * s_px_2;
        class_y++;
    }



    cout << endl << "all result:";
    cout << result[0] << " " << result[1] << endl;

    for (i = 0; i < 2; i++) {
        class_y = i;
        if (result[i] < result[i + 1]) {
            class_y = i + 1;
        }
    }

    cout << "(" << s_x1 << "," << s_x2 << ")所属的类是: " << class_y + 1 << endl;

    return 0;
}