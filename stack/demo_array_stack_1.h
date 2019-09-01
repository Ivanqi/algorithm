#ifndef ARRAY_STACJ_H
#define ARRAY_STACJ_H

typedef struct _array_stack
{
    int size;   // 栈的大小
    int pos;    // 当前存储元素的个数，即栈顶元素下表
    int *array; // 数据存储区
}arrayStack;

#define _size(arrayStack) (arrayStack->size)
#define _is_empty(arrayStack) (arrayStack->pos == -1)
#define _is_full(arrayStack)  (arrayStack->pos == (arrayStack->size - 1))

#endif