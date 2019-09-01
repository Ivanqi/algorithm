#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 逆波兰表示法
int top, S[1000];

void push(int x) {
    // top 加1 之后将元素插入 top 所指向的位置
    S[++top] = x;
}

int pop() {
    top--;
    // 返回top所指的元素
    return S[top + 1];
}

// input: 1 2 + 3 4 - * output: -3
int main () {

    int a, b;
    top = 0;
    char s[100];

    while (scanf("%s", s) != EOF) {
        if (s[0] == '+') {
            a = pop();
            b = pop();
            push(a + b);
        } else if (s[0] == '-') {
            b = pop();
            a = pop();
            push(a - b);
        } else if (s[0] == '*') {
            a = pop();
            b = pop();
            push(a * b);
        } else {
            push(atoi(s));
        }
    }

    printf("%d\n", pop());
    return 0;
}