#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo_doubly_linked_list_3.h"
// 双向链表结点
typedef struct My_node {
    struct My_node *prev;
    struct My_node *pNext;
    void* p;
} my_node;

// 表头不存放元素值
my_node *phead = NULL;

// 结点的个数
int node_count = 0;

// 创建结点，成功返回结点指针，否则返回NULL
my_node* create_node(void *pVal) {
    my_node *pnode = NULL;
    pnode = (my_node*)malloc(sizeof(my_node));

    if (!pnode) {
        printf("create pnode error\n");
        return NULL;
    }
    // 默认的，pnode的前一结点和后一结点都指向他自己
    pnode->prev = pnode->pNext = pnode;
    // 结点的值为pVal
    pnode->p = pVal;
    return pnode;
}

// 新建双向链表。成功返回0， 否则返回-1
int create_dLink() {
    phead = create_node(NULL);
    if (!phead) {
        return -1;
    }

    // 设置结点的个数
    node_count = 0;
}

// 销毁双向链表
int destory_dLink() {
    if (!phead) {
        printf("%s failed! dlink is null \n", __func__);
        return -1;
    }
    my_node *pnode = phead->pNext;
    my_node *ptmp = NULL;

    if (pnode != phead) {
        ptmp = pnode;
        pnode = pnode->pNext;
        free(pnode);
    }

    free(phead);
    phead = NULL;
    node_count = 0;
    return 0;
}

int is_empty_dLink() {
    return node_count == 0;
}

int dLink_size() {
    return node_count;
}

// 获取双向链表中第Index位置的结点
my_node* get_node(int index) {
    if (index < 0 || index >= node_count) {
        printf("%s failed ! index out of bound \n", __func__);
        return NULL;
    }

    // 正向查找
    if (index <= (node_count / 2)) {
        int i = 0;
        my_node *pnode = phead->pNext;
        while ((i++) < index) {
            pnode = pnode->pNext;
        }
        return pnode;
    }

    // 反向查找
    int j = 0;
    int rindex = node_count - index - 1;
    my_node *rnode = phead->prev;

    while ((j++) < rindex) {
        rnode = rnode->prev;
    }
    return rnode;
}

void* dLink_get(int index) {
    my_node *pindex = get_node(index);
    if (!pindex) {
        printf("%s failed \n", __func__);
        return NULL;
    }
    return pindex->p;
}

// 获取第一个结点
void* dLink_getFist() {
    return get_node(0);
}

// 获取最后一个结点
void* dLink_getTail() {
    return get_node(node_count - 1);
}

// 将值插入到index位置，成功返回0，否则返回-1
int dLink_insert(int index, void* pVal) {
    // 插入表头
    if (index == 0) {
        return dLink_insert_head(pVal);
    }

    // 获取要插入位置对应的结点
    my_node *pindex = get_node(index);
    if (!pindex) return -1;

    // 创建结点
    my_node *pnode = create_node(pVal);
    if (!pnode) return -1;

    pnode->prev = pindex->prev;
    pnode->pNext = pindex;

    pindex->prev->pNext = pnode;
    pindex->prev = pnode;
    node_count++;
    return 0;
}

// 数值插入表头
int dLink_insert_head(void* pVal) {

    my_node* pnode = create_node(pVal);
    if (!pnode) {
        return -1;
    }

    pnode->prev = phead;
    pnode->pNext = phead->pNext;

    phead->pNext->prev = pnode;
    phead->pNext = pnode;
    node_count++;

    return 0;
}

// 数值插入表尾
int dLink_insert_tail(void* pVal) {
    my_node* pnode = create_node(pVal);
    if (!pnode) return -1;

    pnode->pNext = phead;
    pnode->prev = phead->prev;

    phead->prev->pNext = pnode;
    phead->prev = pnode;

    return 0;
}

int dLink_delete(int index) {
    my_node* pindex = get_node(index);
    if (!pindex) {
        printf("%s failed! the index in out of bound\n", __func__);
        return -1;
    }

    pindex->pNext->prev = pindex->prev;
    pindex->prev->pNext = pindex->pNext;

    free(pindex);
    node_count--;
    return 0;
}

int dLink_delet_fisrt() {
    return dLink_delete(0);
}

int dLink_delete_tail() {
    return dLink_delete(node_count - 1);
}


//1.双向链表操作数为int
void int_test() {
    int arr[10] = {11,55,67,90,21,45,23,59,79,10};
    printf("xxxxxxxxxxxxxxxxx\n");
    create_dLink();                    //创建链表
    dLink_insert(0, &arr[0]);        //双向链表表头插入
    dLink_insert(0, &arr[1]);        //双向链表表头插入
    dLink_insert(0, &arr[2]);        //双向链表表头插入
    dLink_insert(0, &arr[3]);        //双向链表表头插入
    dLink_insert(0, &arr[4]);        //双向链表表头插入
    dLink_insert(0, &arr[5]);        //双向链表表头插入
    printf("is_empty_dLink()=%d\n",is_empty_dLink());    //双向链表是否为空
    printf("dLink_size()=%d\n", dLink_size());                    //双向链表的大小
    //遍历双向链表
    int i ;
    int * p ;
    int sz = dLink_size();
    for ( i = 0; i < sz; i++) {
        p = (int*)dLink_get(i);
        printf("dLink_get(%d)=%d\n",i,*p);
    }
    destory_dLink();
}

//2.操作数为字符串
void string_test() {
    char* str[] = {"one","two","three","four","five"};
    create_dLink();                    //创建链表
    dLink_insert(0, str[0]);        //双向链表表头插入
    dLink_insert(0, str[1]);        //双向链表表头插入
    dLink_insert(0, str[2]);        //双向链表表头插入
    printf("is_empty_dLink()=%d\n", is_empty_dLink());    //双向链表是否为空
    printf("dLink_size()=%d\n", dLink_size());            //双向链表的大小
    //遍历双向链表
    int i ;
    char * p ;
    int sz = dLink_size();
    for (i = 0; i < sz; i++)
    {
        p = (char*)dLink_get(i);
        printf("dLink_get(%d)=%s\n", i, p);
    }
    destory_dLink();
}
//3.双向链表为结构体
typedef struct MyStruct {
    int id;
    char name[20];
} stu;

stu arr_stu[] = {
    {1000,"lii"},
    { 1001,"mike" },
    { 1002,"lucky" },
    { 1003,"eric" },
};

#define arr_stu_size  ((sizeof(arr_stu))/(sizeof(arr_stu[0])))

void stuc_test() {
    create_dLink();                    //创建链表
    dLink_insert(0, &arr_stu[0]);        //双向链表表头插入
    dLink_insert(0, &arr_stu[1]);        //双向链表表头插入
    dLink_insert(0, &arr_stu[2]);        //双向链表表头插入
    printf("is_empty_dLink()=%d\n", is_empty_dLink());    //双向链表是否为空
    printf("dLink_size()=%d\n", dLink_size());                    //双向链表的大小
    //遍历双向链表
    int i ;
    stu * p ;
    int sz = dLink_size();
    for (i = 0; i < sz; i++) {
        p = (stu*)dLink_get(i);
        printf("dLink_get(%d)=[%d,%s]\n", i, p->id,p->name);
    }
    destory_dLink();
}

int main() {
    int_test();
    string_test();
    stuc_test();
    
    return 0;
}