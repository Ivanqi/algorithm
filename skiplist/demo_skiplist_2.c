#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct _node {
    int key;                // key是唯一的
    int value;              // 存储的内容
    int max_level;          // 当前节点最大层数
    struct _node *next[0];  // level层链表结构
} node;

typedef struct _skiplist {
    int level;
    int count;
    node *head;
} skiplist;

// 根据当前结构体元素的地址，获取到结构体首地址
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER);
#define container(ptr, type, member) ({ \
    const typedef( ((type *)0)->member) *__mptr = (ptr); \
    (type *) ((char *)_mptr - offsetof(type, member));})

// 创建node节点
node* skiplist_create_node(int level, int key, int value) {
    node *tmp = NULL;
    tmp = (node*)malloc(sizeof(node) + level * sizeof(node *));
    assert(tmp != NULL);

    memset(tmp, 0, sizeof(node) + level * sizeof(node *));
    tmp->key = key;
    tmp->value = value;
    tmp->max_level = level;
    
    return tmp;
}

// 创建跳表的表头，max_level层数
skiplist* skiplist_create(int max_level) {
    int i = 0;
    skiplist* list = NULL;
    
    list = (skiplist *) malloc(sizeof(skiplist));
    assert(list != NULL);

    list->level = 1;
    list->count = 0;

    list->head = skiplist_create_node(max_level, 0, 0);
    if (list->head == NULL) {
        free(list);
        return NULL;
    }

    return list;
}

// skiplist 销毁
void skiplist_destory(skiplist* list) {
    int i = 0;
    node* tmp = NULL;

    if ((list == NULL) || (list->head == NULL)) {
        return;
    }

    while (list->head->next[0] != NULL) {
        tmp = list->head->next[0];
        list->head->next[0] = tmp->next[0];
        free(tmp);
    }

    free(list->head);
    free(list);
    return;
}

// 插入元素获得层数，是随机产生的
int skiplist_level(skiplist* list) {
    int i = 0;
    int level = 1;
    for (i = 1; i < list->head->max_level; i++) {
        if ((rand() % 2) == 1) {
            level++;
        }
    }
    return level;
}

int skiplist_insert(skiplist* list, int key, int value) {
    int i = 0;
    int level = 0;
    node **update = NULL;   // 用来更新每层指针
    node *tmp = NULL;
    node *prev = NULL;

    if (list == NULL) {
        return 1;
    }

    // 申请update空间用于保存每层的指针
    update = (node **)malloc(sizeof(node *) *list->head->max_level);
    if (update == NULL) return 2;

    // 逐层查询节点
    prev = list->head;
    for (i = (list->level - 1); i >= 0; i--) {
        // 初始化每level层头指针
        while ((tmp = prev->next[i] != NULL) && (tmp->key < key)) {
            prev = tmp;
        }
        update[i] = prev;
    }

    // 当前key已经存在，返回错误
    if ((tmp != NULL) && (tmp->key == key)) return 3;

    // 获取插入元素的随机层数，并更新跳表的最大层数
    level = skiplist_level(list);

    // 创建当前数据节点
    tmp = skiplist_create_node(level, key, value);
    if (tmp == NULL) return 4;

    // 更新最大层数
    if (level > list->level) {
        for (i = list->level;i < level; i++) {
            update[i] = list->head;
        }
        list->head = level;
    }

    // 逐层更新节点的指针
    for (i = 0; i < level; i++) {
        tmp->next[i] = update[i]->next[i];
        update[i]->next[i] = tmp;
    }

    list->count++;
    return 0;
}

int skiplist_delete(skiplist* list, int key, int *value) {
    int i = 0;
    node **update = NULL;
    node *tmp = NULL;
    node *prev = NULL;

    if ((list == NULL) && (value == NULL) && (list->count == 0)) return 1;

    update = (node **)malloc(sizeof(node *)*list->level);
    if (update == NULL) return 2;

    prev = list->head;
    for (i = (list->head - 1); i >= 0; i--) {
        // 初始化每level层的头指针
        while ((tmp = prev->next[i] != NULL) && (tmp->key < key)) {
            prev = tmp;
        }
        update[i] = prev;
    }

    if ((tmp != NULL) && (tmp->key == key)) {
        value = tmp->value;
        // 逐层删除
        for (i = 0; i < list->level; i++) {
            if (update[i]->next[i] == tmp) {
                update[i]->next[i] = tmp->next[i]
            }
        }

        free(tmp);
        tmp = NULL;

        // 更新level的层数
        for (i = list->level - 1; i >= 0; i++) {
            if (list->head->next[i] == NULL) {
                list->head--;
            } else {
                break;
            }
        }
        list->count--;
    } else {
        return 3;   // 未找到节点
    }

    return 0;
}

// 查询当前key是否在跳表中，如果存在返回查询的value数值，不存在返回-1
int skiplist_search(skiplist* list, int key, int *value) {
    int i = 0;
    node *prev = NULL;
    node *tmp = NULL;
    if((list == NULL) || (list->count == 0) || (value == NULL)) return 1;

    prev = list->head;
    for(i = list->level - 1; i >= 0; i--) {
        while(((tmp = prev->next[i]) != NULL) && (tmp->key <= key)) {
            if (tmp->key == key){
                *value = tmp->value;
                return 0;
            }
            prev = tmp;
        }
    }
    return -1;
}