#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef struct _node {
    int val;
    int max_level;
    struct _node* next[0];
} node;

typedef struct _skiplist {
    int level;
    int count;
    node *head;
} skiplist;


// 创建skiplist结点
node* skiplist_create_node(int level, int value) {
    node* tmp = NULL;
    tmp = (node *)malloc(sizeof(node) + sizeof(node) * level);
    assert(tmp != NULL);

    memset(tmp, 0, sizeof(node) + level * sizeof(node *));
    tmp->val = value;
    tmp->max_level = level;
    return tmp;
}

// skiplist建立
skiplist* skiplist_create(int max_level) {
    skiplist* sk = NULL;
    sk = (skiplist *)malloc(sizeof(skiplist));
    assert(sk != NULL);
    sk->head = skiplist_create_node(max_level, 0);
    if (sk->head == NULL) {
        free(sk->head);
        return NULL;
    }
    sk->level = 1;
    sk->count = 0;
    return sk;
}

// skipllist level
int skiplist_level(int max_level) {
    int level = 1;
    int i;
    for (i = 1; i < max_level;i++) {
        if (rand() % 2 == 1) level++;
    }

    return level;
}

// 销毁skiplist
void skiplist_destory(skiplist* list) {

    if ((list == NULL) || (list->head == NULL)) {
        return;
    }
    
    node* tmp;
    while (list->head->next[0] != NULL) {
        tmp = list->head->next[0];
        list->head->next[0] = tmp->next[0];
        free(tmp);
    }
    free(list->head);
    free(list);
}

/**
 * 1. 逐层检查结点：是为了让插入的值插入到合适的位置，比如前面的值比插入的小
 * 2. 更新最大层数：新生成的level 和 旧level比较。如果新level比旧level大，就必须初始化旧level后面的数据
 * 3. 逐层更新节点的指针：update[i]->next[i] 指向一个新的空地址。然后与tmp的内存地址交换
 * 4. 例子
 *  1. 插入的数据 1, 2, 3 . 对应的level 4, 3, 4
 *  2. 步骤
 *      1. 插入1， level 4
 *         1. node(0) -> 1 -> null
 *         2. node(1) -> 1 -> null
 *         3. node(2) -> 1 -> null
 *         4. node(3) -> 1 -> null
 *      2. 插入2， level 3
 *         1. node(0) -> 1 -> 2 -> null
 *         2. node(1) -> 1 -> 2 -> null
 *         3. node(2) -> 1 -> 2 -> null
 *         4. node(3) -> 1 -> null
 *      3. 插入3， level 4
 *         1. node(0) -> 1 -> 2 -> 3 -> null
 *         2. node(1) -> 1 -> 2 -> 3 -> null
 *         3. node(2) -> 1 -> 2 -> 3 -> null
 *         4. node(3) -> 1 -> 3 -> null
 * 
 */

int skiplist_insert(skiplist* list, int val) {
    
    int i = 0;
    int level = 0;
    node* tmp = NULL;
    node** update = NULL;
    node* prev = NULL;

    update = (node **)malloc(sizeof(node) * list->head->max_level);
    if (update == NULL) return -1;

    prev = list->head;

    // 逐层检查结点
    for (i = list->level -1 ; i >= 0; i--) {
        while (((tmp = prev->next[i]) != NULL) && (tmp->val < val)) {
            prev = tmp;
        }
        update[i] = prev;
    }

    if ((tmp != NULL) && (tmp->val == val)) return -1;

    level = skiplist_level(list->head->max_level);
    tmp = skiplist_create_node(level, val);
    assert(tmp != NULL);

    // 更新最大层数
    if (level > list->level) {
        for (i = list->level; i < level; i++) {
            update[i] = list->head;
        }
        list->level = level;
    }

    // 逐层更新节点的指针
    for (i = 0; i < level; i++) {
        tmp->next[i] = update[i]->next[i];
        update[i]->next[i] = tmp;
    }
    list->count++;
    return 0;
}

void skiplist_dump(skiplist* list) {
    assert(list != NULL);

    int i;
    node* tmp;

    printf("\n -------------- ");
    printf("\n skip list level[%d],count[%d]",list->level,list->count);

    for (i = list->level - 1; i >= 0; i--) {
        tmp = list->head->next[i];
        printf("\n level[%d]: ", i);
        while (tmp) {
            printf("%d ", tmp->val);
            tmp = tmp->next[i];
        }
    }
    printf("\n -------------- \n");
}

int skiplist_search(skiplist *list, int val) {
    
    assert(list != NULL);
    assert(val != 0);

    node* tmp = NULL;
    tmp = list->head;
    int i = 0;

    for (list->level - 1; i >=0; i--) {
        while ((tmp->next[i] != NULL) && (tmp->val <= val)) {
            if (tmp->val == val) {
                return 0;
            }
            tmp = tmp->next[i];
        }
    }
    return -1;
}

int skiplist_delete(skiplist *list, int val) {
    assert(list != NULL);
    assert(val != 0);

    int i = 0;
    node **update = NULL;
    node *tmp = NULL;
    node *prev = NULL;

    update = (node **)malloc(sizeof(node *)*list->level);
    assert(update != NULL);


    prev = list->head;
    // 逐层检查结点
     for (i = (list->level - 1); i >= 0; i--) {
       // 初始化每level层的头指针
        while (((tmp = prev->next[i]) != NULL) && (tmp->val < val)) {
            prev = tmp;
        }
        update[i] = prev;
    }

    if ((tmp != NULL) && (tmp->val == val)) {
        // 逐层删除
        for (i = 0; i < list->level; i++) {
            if (update[i]->next[i] == tmp) {
                update[i]->next[i] = tmp->next[i];
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
        return -1;
    }

    return 0;
} 

int main() {
    int res = 0;
    int key = 0;
    int value = 0;

    skiplist *list = NULL;

    list = skiplist_create(5);
    assert(list != NULL);

    printf("\n ---- insert ----- \n");
    int i;
    for (i = 1; i <= 20; i++) {
        skiplist_insert(list, i);
    }

    skiplist_dump(list);

    if (skiplist_search(list, 5) == 0) {
        printf("寻找到：5\n");
    } else {
        printf("寻找不到：5\n");
    }

    if (skiplist_search(list, 22) == 0) {
        printf("寻找到：22\n");
    } else {
        printf("寻找不到： 22\n");
    }


    if (skiplist_delete(list, 14) == 0) {
        printf("删除：14 成功\n");
    } else {
        printf("删除：14 失败\n");
    }

    if (skiplist_delete(list, 22) == 0) {
        printf("删除：22 成功\n");
    } else {
        printf("删除：22 失败\n");
    }
    skiplist_dump(list);

    skiplist_destory(list);
    return 0;
}