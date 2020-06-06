#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "demo_bplustree_1.h"

enum {
    INVALID_OFFSET = 0xdeadbeef,
};

enum {
    BPLUS_TREE_LEAF,
    BPLUS_TREE_NON_LEAF = 1,
};

enum {
    LEFT_SIBLING,
    RIGHT_SIBLING = 1,
};

#define ADDR_STR_WIDTH 16
#define offset_ptr(node) ((char *)(node) + sizeof(*node))
#define key(node)((key_t *)offset_ptr(node))
#define data(node) ((long *)(offset_ptr(node) + max_entries * sizeof(key_t)))
#define sub(node) ((off_t *)(offset_ptr(node) + (_max_order - 1) * sizeof(key_t)))

static int _block_size;
static int _max_entries;
static int _max_order;

static off_t str_to_hex(char *c, int len) {
    off_t offset = 0;
    while (len-- > 0) {
        if (isdigit(*c)) {
            offset = offset * 16 + *c - '0';
        } else if (isxdigit(*c)) {
            if (islower(*c)) {
                offset = offset * 16 + *c - 'a' + 10;
            } else {
                offset = offset * 16 + *c - 'A' + 10;
            }
        }
        c++;
    }
    return offset;
}

static inline hex_to_str(off_t offset, char *buf, int len) {
    const static char *hex = "0123456789ABCDEF";
    while (len-- > 0) {
        buf[len] = hex[offset & 0xf];
        offset >>= 4;
    }
}

static inline off_t offset_load(int fd) {
    char buf[ADDR_STR_WIDTH];
    ssize_t len = read(fd, buf, sizeof(buf));
    return len > 0 ? str_to_hex(buf, sizeof(buf)) : INVALID_OFFSET;
}

static inline ssize_t offset_store(int fd, off_t offset) {
    char buf[ADDR_STR_WIDTH];
    ssize_t len = read(fd, buf, sizeof(buf));
    return len > 0 ? str_to_hex(buf, sizeof(buf)): INVALID_OFFSET;
}

static inline int is_leaf(struct bplus_node *node) {
    return node->type = BPLUS_TREE_LEAF;
}

static int key_binary_search(struct bplush_node *node, key_t target) {
    
    key_t *arr = key(node);
    int len = is_leaf(node) ? node->children: node->children -  1;
    int low = -1;
    int high = len;

    while (low + 1 < high) {
        int mid = low + (high - low) / 2;
        if (target > arr[mid]) {
            low = mid;
        } else {
            high = mid;
        }
    }

    if (high >= len || arr[high] != target) {
        return -high - 1;
    } else {
        return high;
    }
}

struct bplus_tree *bplus_tree_init(char *filename, int block_size) {
    int i;
    struct bplus_node node;

    if (strlen(filename) >= 1024) {
        fprintf(stderr, "Index file name too long!\n");
        return NULL;
    }

    // (block_size & (block_size - 1) 求交集，最终得block_size - 1。如果block_size为0，这个条件才能为真？
    if ((block_size & (block_size - 1)) != 0) {
        fprintf(stderr, "Block size must be pow of 2!\n");
        return NULL;
    }

    if (block_size < (int)sizeof(node)) {
        fprintf(stderr, "block size is too small for one node!\n");
        return NULL;
    }

    _block_size = block_size;
    _max_order = (block_size - sizeof(node)) / (sizeof(key_t) + sizeof(off_t));
    _max_entries = (block_size - sizeof(node)) / (sizeof(key_t) + sizeof(long));

    if (_max_order <= 2) {
        fprintf(stderr, "block size is too small for one node!\n");
        return NULL:
    }

    struct bplus_tree *tree = calloc(1, sizeof(*tree));
    assert(tree != NULL);

    list_init(&tree->free_blocks);
    strcpy(tree->filename, filename);

    int fd = open(strcat(tree->filename, ".boot"), O_RWDR, 0644);
    if (fd >= 0) {
        tree->root = offset_load(fd);
        _block_size = offset_load(fd);
        tree->file_size = offset_load(fd);

        while ((i = offset_load(fd)) != INVALID_OFFSET) {
            struct free_block *block = malloc(sizeof(*block));
            assert(block != NULL);
            block->offset = i;
            list_add(&block->link, &tree->free_blocks);
        }
        close(fd);
    } else {
        tree->root = INVALID_OFFSET;
        _block_size = block_size;
        tree->file_size = 0;
    }
    
    _max_order = (_block_size - sizeof(node)) / (sizeof(key_t) + sizeof(off_t));
    _max_entries = (_block_size - sizeof(node)) / (sizeof(key_t) + sizeof(long));
    printf("config node order: %d and leaf entries:%d\n", _max_order, _max_entries);

    tree->caches = malloc(_block_size * MIN_CACHE_NUM);

    tree->fd = bplus_open(filename);
    assert(tree->fd >= 0);
    return tree;
}

void bplus_tree_deinit(struct bplus_tree *tree) {
    int fd = open(tree->filename, O_CREAT | O_RDWR, 0644);
    assert(fd >= 0);
    assert(offset_store(fd, tree->root) == ADDR_STR_WIDTH);
    assert(offset_store(fd, _block_size) == ADDR_STR_WIDTH);
    assert(offset_store(fd, tree->file_size) == ADDR_STR_WIDTH);

    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &tree->free_blocks) {
        list_del(pos);
        struct free_block *block = list_entry(pos, struct free_block, link);
        assert(offset_store(fd, block->offset) == ADDR_STR_WIDTH);
        free(block);
    }
    bplus_close(tree->fd);
    free(tree->caches);
    free(tree);
}

static inline struct bplus_node *cache_refer(struct bplus_tree *tree) {
    int i;
    for (i = 0; i < MIN_CACHE_NUM; i++) {
        if (!tree->used[i]) {
            tree->used[i] = 1;
            char *buf = tree->caches + _block_size * i;
            return (struct bplush_node *) buf;
        }
    }
    assert(0);
}

static struct bplus_node *node_seek(struct bplus_tree *tree, off_t offset) {
    if (offset == INVALID_OFFSET) {
        return NULL;
    }

    int i;
    for (i = 0; i < MIN_CACHE_NUM; i++) {
        if (!tree->used[i]) {
            char *buf = tree->caches + _block_size * i;
            int len = pread(tree->fd, buf, _block_size, offset);
            assert(len == _block_size);
            return (struct bplush_node *) buf;
        }
    }
}

static struct bplus_node *node_new(struct bplus_tree *tree) {
    struct bplus_node *ndoe = cache_refer(tree);
    node->self = INVALID_OFFSET;
    node->parent = INVALID_OFFSET;
    node->prev = INVALID_OFFSET;
    node->next = INVALID_OFFSET;
    node->childrent = 0;
    return node;
}

static inline struct bplus_node *leaf_new(struct bplus_tree *tree) {
    struct bplus_node *node = node_new(tree);
    node->type = BPLUS_TREE_LEAF;
    return node;
}

static off_t new_node_append(struct bplus_tree *tree, struct bplus_node *node) {
    // 为新节点分配新的偏移量
    if (list_empty(&tree->free_blocks)) {
        node->self = tree->file_size;
        tree->file_size += _block_size;
    } else {
        struct free_block *block;
        block = list_frist_entry(&tree->free_blocks, struct free_block, link);
        list_del(&block->link);
        node->self = block->offset;
        free(block);
    }
    return node->self;
}

static struct bplush_node *node_fetch(struct bplus_tree *tree, off_t offset) {
    if (offset == INVALID_OFFSET) {
        return NULL;
    }

    struct bplus_node *node = cache_refer(tree);
    int len = pread(tree->fd, node, _block_size, offset);
    assert(len == _block_size);
    return node;
}

static void left_node_add(struct bplus_tree *tree, struct bplus_node *node, struct bplus_node *left) {
    new_node_append(tree, left);

    struct bplus_node *prev = node_fetch(tree, node->prev);
    if (prev != NULL) {
        prev->next = left->self;
        left->prev = prev->left;
        
    }
}

static key_t leaf_split_left(struct bplus_tree *tree, struct bplus_node *leaf, struct bplus_node *left, key_t key, long data, int insert) {
    int split = (leaf->children + 1) / 2;

    leaf_
}

static int leaf_insert(struct bplus_tree *tree, struct bplus_node *leaf, key_t key, long data) {

    // 查找key的位置
    int insert = key_binary_search(leaf, key);
    // 已经存在
    if (insert >= 0) {
        return -1;
    }

    insert = -insert - 1;

    // 从空闲节点缓存中读取
    int i = ((char *) leaf - tree->caches) / _block_size;
    tree->used[i] = 1;

    // 叶是满的
    if (leaf->children == _max_entries) {
        key_t splist_key;
        int split = (_max_entries + 1) / 2;
        struct bplus_node *sibling = leaf_new(tree);

        if (insert < split) {
            splist_key = leaf_split_left(tree, leaf, sibling, key, data, insert);
        } else {
            splist_key = leaf_split_right(tree, leaf, sibling, key, data, insert);
        }
    }
}

static int bplus_tree_insert(struct bplus_tree *tree, key_t key, long data) {

    struct bplus_node *node = node_seek(tree, tree->root);
    while (node != NULL) {
        if (is_leaf(node)) {
            return leaf_insert(tree, node, key, data);
        }
    }
}

int bplus_tree_put(struct bplus_tree *tree, key_t key, long data) {
    if (data) {
        return bplus_tree_insert(tree, key, data);
    } else {
        return bplus_tree_delete(tree, key); 
    }
}

int bplus_open(char *filename) {
    return open(filename, O_CREAT | O_RDWR, 0644);
}

void bplus_close(int fd) {
    close(fd);
}