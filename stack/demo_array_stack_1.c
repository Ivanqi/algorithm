#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo_array_stack_1.h"

// 创建并初始化顺序栈
arrayStack *create(int size) {

    arrayStack *stack = (arrayStack *)malloc(sizeof(arrayStack));
    if (stack == NULL) {
        return NULL;
    }

    stack->size = size;
    stack->pos = -1;
    stack->array = (int *)malloc(sizeof(int) * size);

    if (stack->array == NULL) {
        free(stack);
        return NULL;
    }
    return stack;
}

// 销毁顺序栈
void destory(arrayStack *stack) {

    if (stack == NULL) {
        return;
    }

    if (stack->array == NULL) {
        free(stack->array);
    }
    free(stack);
    return;
}

// 入栈
int push(arrayStack *stack, int data) {

    if (stack == NULL) {
        return -1;
    }

    if (_is_full(stack)) {
        return -1;
    }

    stack->pos++;
    stack->array[stack->pos] = data;
    return 0;
}

// 入栈(可扩容)
int push_dilatation(arrayStack *stack, int data) {
    
    if (stack == NULL) {
        return -1;
    }

    // 如果栈不满，直接插入
    if (!_is_full(stack)) {
        return push(stack, data);
    }

    // 如果栈已经满，申请内存
    int *ptmp = (int *)malloc(2 * sizeof(int) * stack->size);

    if (ptmp == NULL) {
        return -1;
    }

    memcpy(ptmp, stack->array, stack->size * sizeof(int));
    free(stack->array);

    stack->array = ptmp;
    stack->size = 2 * stack->size;
    stack->pos++;
    stack->array[stack->pos] = data;
}

// 出栈
int pop(arrayStack *stack) {

    int data = 0;
    if (_is_empty(stack)) {
        return -1;
    }

    data = stack->array[stack->pos];
    stack->pos--;
    return data;
}

void print(arrayStack *stack) {

    if (stack == NULL) {
        printf("\r\n Stack Is NULL.");
        return;
    }

    if (_is_empty(stack)) {
        printf("\r\n Stack is empty");
        return;
    }

    printf("\r\n Stack size = %d, pos = %d", stack->size, stack->pos);

    int i = 0;
    for (i = 0; i <= stack->pos; i++) {
        printf("\r\n array[%d] = %d", i, stack->array[i]);
    }
    printf("\r\n");
}

void test_stack() {
    
    arrayStack *stack = NULL;
    stack = create(4);

    if (stack == NULL) {
        printf("\n Stack is NULL \n");
    }
    int i = 1;
    for (i = 1; i < 5; i++) {
        if (push(stack, i) != 0) {
            printf("\r\n %d failed to push the Stack \r\n", i);
        }
    }

    print(stack);
    push(stack, 5);
    pop(stack);
    print(stack);
    destory(stack);
}

void test_stack_dilatation()
{
    arrayStack *stack = NULL;
    stack = create(4);

    if (stack == NULL) {
        printf("\n Stack is NULL \n");
    }
    int i = 1;
    for (i = 1; i < 5; i++) {
        if (push(stack, i) != 0) {
            printf("\r\n %d failed to push the Stack \r\n", i);
        }
    }

    print(stack);
    push(stack, 5);
    pop(stack);
    print(stack);

    for (i = 5; i < 10; i++) {
        if (push_dilatation(stack, i) != 0) {
            printf("\r\n %d failed to push the Stack \r\n", i);
        }
    }
    print(stack);
    destory(stack);
}

int main () {
    
    test_stack_dilatation();
    return 0;
}
