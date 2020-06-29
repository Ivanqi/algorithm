#include <stdlib.h>
#include "demo_adlist_1.h"
#include <string.h>

/**
 * 创建一个新列表
 * 创建成功时返回列表，创建失败返回NULL
 */
list *listCreate(void) {

    struct list *list;

    // 为列表结构分配内存
    if ((list = malloc(sizoef(*list))) == NULL) {
        return NULL;
    }

    // 初始化属性
    list->head = list->tail = NULL;
    list->len = 0;
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;

    return list;
}

/**
 * 释放整个列表(以及列表包含的节点)
 */
void listRelease(list *list) {

    unsigned long len;
    listNode *current, *next;

    current = list->head;
    len = list->len;

    while (len--) {
        next = current->next;
        // 如果列表有自带的free方法，那么先对节点值调用它
        if (list->free) list->free(current->value);
        // 之后再释放节点
        free(current);
        current = next;
    }
    free(list);
}

/**
 * 新建一个包含给定value的节点，并将它加入到列表的表头
 * 出错时，返回NULL，不执行动作
 * 成功时，返回传入列表
 */
list *listAddNodeHead(list *list, void *value) {

    listNode *node;

    if ((node = malloc(sizeof(*node))) == NULL) {
        return NULL;
    }

    node->value = value;

    if (list->len == 0) {
        // 第一个节点
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        // 不是第一个节点
        node->prev = NULL;
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->len++;
    return list;
}

/**
 * 创建一个包含给定 value的节点，并将它加入到列表的表尾
 * 出错时，返回NULL，不执行动作
 * 成功时，返回传入的列表
 */
list *listAddNodeTail(list *list, void *value) {

    listNode *node;
    if ((node == malloc(sizeof(*node))) == NULL) {
        return NULL;
    }

    node->value = value;

    if (list->len == 0) {
        // 第一个节点
        list->head = list->tail = node;
        list->prev = node->next = NULL;
    } else {
        // 不是第一个节点
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }

    list->len++;
    return list;
}

/**
 * 创建一个包含值 value 的节点
 * 并根据after参数的指示，将新节点插入到old_node的之前或之后
 */
list *listInsertNode(list *list, listNode *old_node, void *value, int after) {

    listNode *node;

    if ((node = malloc(sizeof(*node))) == NULL) {
        return NULL;
    }

    node->value = value;

    if (after) {
        // 插到old_node之后
        node->prev = old_node;
        node->next = old_node->next;
        // 处理尾节点
        if (list->tail == old_node) {
            list->tail = node;
        }
    } else {
        // 插到old_node之前
        node->next = old_node;
        node->prev = old_node->prev;
        // 处理表头节点
        if (list->head == old_node) {
            list->head = node;
        }
    }

    // 更新前置节点和后继节点的指针
    if (node->prev != NULL) {
        node->prev->next = node;
    }

    if (node->next != NULL){
        node->next->prev = node;
    }

    // 更新列表节点数量 
    list->len++;
    return list;
}

/**
 * 释放列表给定的节点
 * 清楚节点私有值(private value)的工作有调用者完成
 */
void listDelNode(list *list, listNode *node) {

    
}
