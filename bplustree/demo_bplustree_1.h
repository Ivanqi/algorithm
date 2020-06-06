#ifndef _BPLUS_TREE_H
#define _BPLUS_TREE_H

#define MIN_CACHE_NUM 5

#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - (size_t)(&((type *)0)->member)))

#define list_frist_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
    list_entry((ptr)->prev, type, member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

typedef int key_t;

struct list_head {
    struct list_head *prev, *next;
};

static inline void list_init(struct list_head *link)
{
    link->prev = link;
    link->next = link;
}

static inline void __list_add(struct list_head *link, struct list_head *prev, struct list_head *next)
{
    link->next = next;
    link->prev = prev;
    next->prev = link;
    prev->next = link;
}

static inline void list_add(struct list_head *link, struct list_head *prev)
{
    __list_add(link, prev, prev->next);
}

static inline void list_add_tail(struct list_head *link, struct list_head *prev)
{
    __list_add(link, head->prev, head);
}

static inline void list_del(struct list_head *link)
{
    __list_del(link->prev, link->next);
    list_init(link);
}

static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

typedef struct bplus_node {
    off_t self;
    off_t parent;
    off_t prev;
    off_t next;
    int type;
    /**
     * 如果是叶节点，则它指定条目数
     * 如果是非叶子节点，则指定子节点数
     */
    int children;
} bplus_node;

typedef struct free_block {
    struct list_head link;
    off_t offset;
} free_block;

struct bplus_tree {
    char *caches;
    int used[MIN_CACHE_NUM];
    char filename[1024];
    int fd;
    int level;
    off_t root;
    off_t file_size;
    struct list_head free_blocks;
};

void bplus_tree_dump(struct bplus_tree *tree);
long bplus_tree_get(struct bplus_tree *tree, key_t key);
int bplus_tree_put(struct bplus_tree *tree, key_t key, long data);
bool bplus_tree_get_range(struct bplus_tree *tree, key_t key1, key_t key2);
struct bplus_tree *bplus_tree_init(char *filename, int block_size);
void bplus_tree_deinit(struct bplus_tree *tree);
int bplus_open(char *filename);
void bplus_close(int fd);


static int bplus_tree_insert(struct bplus_tree *tree, key_t key, long data);
static inline struct bplus_node *leaf_new(struct bplus_tree *tree);
static key_t non_leaf_insert(struct bplus_tree *tree, struct bplus_node *node, struct bplus_node *l_ch, struct bplus_node *r_ch, key_t key);
static struct bplus_node *node_new(struct bplus_tree *tree);
static inline struct bplus_node *cache_refer(struct bplus_tree *tree);
static key_t leaf_split_left(struct bplus_tree *tree, struct bplus_node *leaf, struct bplus_node *left, key_t key, long data, int insert);
static void left_node_add(struct bplus_tree *tree, struct bplus_node *node, struct bplus_node *left);
static off_t new_node_append(struct bplus_tree *tree, struct bplus_node *node);
static struct bplus_node *node_fetch(struct bplus_tree *tree, off_t offset);


#endif