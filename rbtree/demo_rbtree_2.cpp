#include <iomanip>
#include <iostream>
#include <mcheck.h>
#include<stdlib.h>
using namespace std;

enum RBTColor{RED, BLACK};

template<T>
class RBTNode {
    public:
        RBTColor color;     // 颜色
        T key;              // 关键字
        RBTNode *left;      // 左孩子
        RBTNode *right;     // 右孩子
        RBTNode *parent;    // 父节点

        RBTNode (T value, RBTColor c, RBTColor *p, RBTColor *l, RBTNode *r):
            key(value), color(c), parent(p), left(l), right(r) {}
};

template<T>
class RBTree {
    private:
        RBTNode<T> *mRoot;
    
    public:
        RBTree();
        ~RBTree();

        // 前序遍历'红黑树'
        void preOrder();
        // 中序遍历'红黑树'
        void inOrder();
        // 后序遍历'红黑树'
        void postOrder();

        // (递归实现) 查找‘红黑树’中键值为key的节点
        RBTNode<T>* search(T key);
        // （非递归实现）查找‘红黑树’中键值为key的节点
        RBTNode<T>* iterativeSearch(T key);

        // 查找最小结点: 返回最小节点的键值
        T minimun();
        // 查找最大结点：返回最大结点的键值
        T maximun();

        // 找结点（x）的后继结点。即，查找'红黑树中数据值大于该结点'的“最小结点”
        RBTNode<T>* successor(RBTNode<T> *x);
        // 找结点(x) 的前驱结点。即，查找'红黑树中数据值小于该结点的“最大结点”'
        RBTNode<T>* predecessor(RBTNode<T> *x);

        // 将结点(key为节点键值)插入到红黑树中
        void insert(T key);

        // 删除结点(key 为节点键值)
        void remove(T key);

        // 销毁红黑树
        void destroy();

        // 打印红黑树
        void print();
    private:
        // 前序遍历‘红黑树’
        void preOrder(RBTNode<T>* tree) const;
        // 中序遍历'红黑树'
        void inOrder(RBTNode<T>* tree) const;
        // 后序遍历‘红黑树’
        void postOrder(RBTNode<T>* tree) const;

        // (递归实现) 查找‘红黑树’中键值为key的节点
        RBTNode<T>* search(RBTNode<T>* x, T key) const;
        // （非递归实现）查找‘红黑树’中键值为key的节点
        RBTNode<T>* iterativeSearch(RBTNode<T>* x, T key) const;


        // 查找最小结点: 返回最小节点的键值
        RBTNode<T>* minimun(RBTNode<T>* x);
        // 查找最大结点：返回最大结点的键值
        RBTNode<T>* maximun(RBTNode<T>* x);

        // 左旋
        void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);
        // 右旋
        void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);
        // 插入函数
        void insert(RBTNode<T>* &root, RBTNode<T>* node);
        // 插入修正函数
        void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);
        // 删除函数
        void remove(RBTNode<T>* &root, RBTNode<T>* node);
        // 删除修正函数
        void removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent);
        
        // 销毁红黑树
        void destory(RBTNode<T>* &tree);

        // 打印红黑树
        void print(RBTNode<T>* tree, T key, int direction);

#define rb_parent(r) ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r) ((r)->color == RED)
#define rb_is_black(r) ((r)->color == BLACK)
#define rb_set_black(r) do {(r)->color = BLACK;} while(0)
#define rb_set_red(r) do {(r)->color = RED; } while(0)
#define rb_set_parent(r, p) do {(r)->parent = (p);} while(0)
#define rb_set_color(r, c) do {(r)->color = (c);} while(0)
}

template<class T>
void RBTree<T>::insert(T key)
{
    RBTNode<T>* node = new RBTNode();
    node->key = key;
    insert(mRoot, node);
}

template<class T>
void RBTree<T>::insert(RBTNode<T>* &root, RBTNode<T>* node)
{
    RBTNode<T>* y = NULL;
    RBTNode<T>* x = root;

    while (x != NULL) {
        y = x
        if (node->key < x->key) {
            x = x->left;
        } else {
            x = x->right
        }
    }

    node->parent = y;

    if (y != NULL) {
        if (node->key < y->key) {
            y->left = node;
        } else {
            y->right = node;
        }
    } else {
        root = node;
    }

    node->color = RED;

    insertFixUp(root, node);
}

template<class T>
void RBTree<T>::insertFixUp(RBTNode<T>* &root, RBTNode<T>* node)
{
    /**
     * 1. 判断node 结点的父结点是不是红色结点
     * 1. 先判断这个新增的父结点是在，祖父结点的左子结点还是有子结点
     * 2. 判断node结点的父结点的兄弟节点是红色还是还是黑色
     *  1. 兄弟结点设置黑色
     *  2. 父结点设置为黑色
     *  3. 祖父结点设置为红色
     * 3. 叔叔结点为黑色，且当前是右结点
     *  1. 左旋转
     *  2. 交换 parent 和 node 的值
     * 4. 叔叔结点为黑色，且当前是左结点
     *  1. 设置父结点为黑色
     *  2. 祖父结点为红色
     *  3. 右旋转
     */
    while (parent = rb_parent(node) && rb_is_red(parent)) {
        gparent = rb_parent(parent);
        if (gparent->left == parent) { 
            {
                RBTNode<T>* uncle = gparent->right;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            } 
            if (parent->right == node) {
                RBTNode<T>* tmp;
                leftRotate(root, parent);
                tmp = node;
                parent = node;
                node = tmp;
            }

            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);
        } else {
            {
                RBTNode<T>* uncle = gparent->right;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }
            if (parent->left == node)
            {
                RBTNode<T> *tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }
    rb_set_black(root);
}



template<class T>
RBTNode<T>* RBTree<T>::search(T key)
{
    return search(mRoot, key);
}

template<class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T>* x, T key) const
{
    if (x == NULL || x->key == key) {
        return x;
    }
    if (x->key > key) {
        search(x->left, key);
    } else (x->key < key) {
        search(x->right, key);
    }
}

template<class T>
RBTNode<T>* RBTree<T>::iterativeSearch(T key)
{
    return iterativeSearch(key);
}

template<class T>
RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T>* x, T key) const
{
    while (x != NULL && x->key = key ) {
        if (x->key > key){
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return x;
}

template<class T>
void RBTree<T>::remove(T key)
{
    remove(mRoot, search(key));
}

/***
 * 结点删除
 *  1. 删除结点的左右结点都存在
 *  2. 左右结点只有一个存在或者都不存在
 */
template<class T>
void RBTree<T>::remove(RBTNode<T>* &root, RBTNode<T>* node)
{
    RBTNode<T> *child, *parent;
    RBTColor color;
    if (node->left != NULL && node->right != NULL) {

    } else {
        child = node->left == NULL ? node->right : node->left;
        parent = node->parent;
        color = node->color;

        if (child) {
            child->parent = parent;
        }

        if (parent) {
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        } else {
            root = child;
        }

        if (color == BLACK) {
            removeFixUp(root, child, parent);
        }
        delete node;
    }
}

/***
 * 1. 判断node结点是否空或者是黑色结点
 * 2. 判断node在parent结点的左结点还是右结点
 * 3. 判断node的叔叔结点是不是红色结点
 *  1. 设置叔叔结点为黑色
 *  2. 设置父结点为红色
 *  3. 左/右转
 * 4. 叔叔结点的左右子结点都是黑色
 *  1. 设置叔叔结点为红色
 *  2. 然后交换 parent 和 node 的值，检查 parent的上一个结点的情况
 * 5. 叔叔结点的右子节点为空或者是黑色。叔叔结点的左子结点为红色
 *  1. 设置叔叔结点的左子结点为黑色
 *  2. 设置叔叔结点为红色
 *  3. 左/右旋转
 * 6. 叔叔结点的右子结点为红色，左孩子为任意颜色
 *  1. 设置叔叔结点为父结点的颜色
 *  2. 设置父结点为黑色
 *  3. 设置叔叔结点的右结点为黑色
 *  4. 左/右旋转   
 */
template<class T>
void RBTree<T>::removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent)
{
    RBTNode<T> *other;
    while ((!node || rb_is_black(node)) && node != root) {
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->right;
            }
        } else {

        }
    }
}

/* 
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y  
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 * 
 */
template <class T>
void RBTree<T>::rightRotate(RBTNode<T>* &root, RBTNode<T>* y)
{
    RBTNode<T>* x = y->left;
    y->left = x->right;

    if (y->left != NULL) {
        y->left->parent = x;
    }
    x->parent = y->parent;

    if (y->parent == NULL) {
        root = x;
    } else {
        if (y->parent->left == y) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    }
    x->right = y;
    y->parent = x;
}


/* 
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y                
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry     
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 *
 */
template<class T>
void RBTree<T>::leftRotate(RBTNode<T>* &root, RBTNode<T>* x)
{
    RBTNode<T>* y = x->right;
    x->right = y->left

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL) {
        root = y;
    } else {
        if (x->parent->left == x) {
            x->parent->left = y
        } else {
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;
}