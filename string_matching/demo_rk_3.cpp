#include <iostream>
using namespace std;
#define nr 5    // 主串行数
#define nc 5    // 主串列数
#define mr 2    // 模式串行数
#define mc 2    // 模式串列数

int cal_hash_t(int *table, int r, int c, char ch[][mc]) {

    int i, j, value = 0;
    for (i = 0; i < r; ++i) {   // 计算2d模式串的hash值value
        for (j = 0; j < c; ++j) {
            value += table[ch[i][j] - 'a'];
        }
    }
    return value;
}

int cal_hash_s_child(int *table, int i0, int j0, int r, int c, char ch[][nc]) {
    int i, j, hash_value = 0;
    for (i = i0; i < r; ++i) {  // 计算2d子串的hash值value
        for (j = j0; j < c; ++j) {
            hash_value += table[ch[i][j] - 'a'];
        }
    }
    return hash_value;
}

// 对比字符串是否真的一致。因为会存在hash冲突的情况
bool same(char s[][nc], char t[][mc], int i0, int j0) {
    int x = i0, y = j0, i, j;
    for (i = 0; i < mr; ++i, ++x) {
        for (j = 0, y = j0; j < mc; ++j, ++y) { // 记得写y = j0,换行后y复位
            if (s[x][y] != t[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * 步骤
 *  1. 先获取模式串的hash值
 *  2. 然后遍历矩阵，然后把每次遍历的[ (i, j), (i, j + 1), (i + 1, j), (i + 1, j + 1)] 取hash
 *  3. 在模式中hash值和子串hash值一样的情况下，对模式串 [ (i, j), (i, j + 1), (i + 1, j), (i + 1, j + 1)] 和 子串的 [ (i, j), (i, j + 1), (i + 1, j), (i + 1, j + 1)] 一一对比，是否模式串和子串相匹配
 */
bool str_RK_2d(char s[][nc], char t[][mc]) {    // s是主串，t是模式串
    //质数表对应 a - z
    int table[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101}; 

    int i, j, hash_val, value;
    value = cal_hash_t(table, mr, mc, t);   // 计算2d模式串哈希值
    for (i = 0; i < nr - mr + 1; ++i) {     // 行最多nr - mr + 1次比较
        for (j = 0; j < nc - mc + 1; ++j) { // 列最多nc - mc + 1次比较
            hash_val = cal_hash_s_child(table, i, j, mr + i, mc + j, s);    // 计算2d子串哈希值
            if (hash_val == value && same(s, t, i, j)) {
                // 如果2d子串哈希值等于模式串的，且"真的"字符串匹配(避免冲突带来假冲突匹配)
                cout << "找到模式矩阵，其左上角" << i + 1 << " 行，" << j + 1 << " 列. " << endl;
                return true;
            }
        }
    }
    return false;
}

/**
 * 假设二维主串和模式串的维度分别是 m * n 和 i * j
 * 横向在[0, m - i], 纵向在[0, n - j]取起始点
 * 然后取同样子串窗口对比,共有(m - i + 1) * (n - j + 1)个子串
 */
int main() {
    char s[  ][nc] = {
        { 'a', 'b', 'a', 'b', 'a' },
        { 'a', 'b', 'a', 'b', 'a' },
        { 'a', 'b', 'b', 'a', 'a' },
        { 'a', 'b', 'a', 'a', 'b' },
        { 'b', 'b', 'a', 'b', 'a' }
    };

    char t[  ][mc] = {
        { 'a', 'b' },
        { 'b', 'a' }
    };
    str_RK_2d(s, t);


    char a[  ][nc] = {
        { 'd', 'a', 'b', 'c' },
        { 'e', 'f', 'a', 'd' },
        { 'c', 'c', 'a', 'f' },
        { 'd', 'e', 'f', 'c' },
        { 'b', 'b', 'a', 'b' }
    };

    char b[  ][mc] = {
        { 'c', 'a' },
        { 'e', 'f' }
    };
    str_RK_2d(a, b);

    return 0;
}