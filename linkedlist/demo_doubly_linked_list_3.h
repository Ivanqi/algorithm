#ifndef _DOUBLY_LINKED_LIST_
#define _DOUBLY_LINKED_LIST_

// 新建双向链表。成功返回链头，否则，返回NULL
int create_dLink();

// 撤销双向链表，成功返回0，否则返回-1
int destory_dLink();

// 双向链表是否为空，为空返回1，否则返回0
int is_empty_dLink();

// 双向链表大小
int dLink_size();

// 获取索引index位置的元素，成功返回结点指针，否则返回NULL
void* dLink_get(int index);

// 获取双向链表中第一个元素，成功返回结点指针，否则返回NULL
void* dLink_getFirst();

// 获取双向链表中最后一个元素，成功返回结点指针，否则返回NULL
void* dLink_getTail();

// 在Index位置插值value，成功返回0，否则返回-1
int dLink_insert(int index, void* pVal);

// 在表头插入值
int dLink_insert_head(void* pVal);

// 在表尾插入值
int dLink_insert_tail(void* pVal);


// 在index处删除
int dLink_delete(int index);
// 删除第一个结点
int dLink_delete_first();
// 删除最后一个结点
int dLink_delete_tail();
#endif