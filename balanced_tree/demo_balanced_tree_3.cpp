#include <iostream>
#include <iomainip>
using namespace std;
#define TBL_SIZE(a) ( (sizeof(a)) / (sizeof(a[0])))
template<class T>
class AVLTreeNode {
    public:
        T key;  // 关键字
        int height; // 高度
        AVLTreeNode *left; // 左节点
        AVLTreeNode *right; // 有节点
    private
        AVLTreeNode(T val, AVLTreeNode *l, AVLTreeNode *r):
            key(val),height(0),left(l),right(r) {}
};


template<class T>
class AVLTree {
    private:
        AVLTreeNode<T> *mRoot; // 根节点
    public:
        AVLTree();
        ~AVLTree();

        // 获取高
        int height();   
        // 获取树的高
        int max(int a, int b);  
        
        // 前序遍历
        void preOrder();
        // 中序遍历
        void inOrder();
        // 后序遍历
        void postOrder();

        // 递归实现，查找AVL
        AVLTreeNode<T>* search(T key);
        // 非递归实现，查找AVL
        AVLTreeNode<T>* iterativeSearch(T key);

        // 查找最小节点
        T minimum();
        // 查找最大节点
        T maxmun();

        // 插入数据
        void insert(T key);

        // 删除节点
        void remove(T key);

        // 销毁AVL树
        void destory();

        // 打印AVL树
        void print();
    private:
        int height(AVLTreeNode<T>* tree);
        
        void preOrder(AVLTreeNode<T>* tree) const;

        void inOrder(AVLTreeNode<T>* tree) const;

        void postOrder(AVLTreeNode<T>* tree) const;

        AVLTreeNode<T>* search(AVLTreeNode<T>* x, T key) const;

        AVLTreeNode<T>* iterativeSearch(AVLTreeNode<T>* x, T key) const;

        AVLTreeNode<T>* minimum(AVLTreeNode<T>* tree);

        AVLTreeNode<T>* maxmun(AVLTreeNode<T>* tree);

        // LL：左左对应的情况(左单旋转)。
        AVLTreeNode<T>* leftLeftRotation(AVLTreeNode<T>* k2);

        // RR：右右对应的情况(右单旋转)。
        AVLTreeNode<T>* rightRightRotation(AVLTreeNode<T>* k1);

        // LR：左右对应的情况(左双旋转)。
        AVLTreeNode<T>* leftRightRotation(AVLTreeNode<T>* k3);

        // RL：右左对应的情况(右双旋转)。
        AVLTreeNode<T>* rightLeftRotation(AVLTreeNode<T>* k4);

        // 将结点(z)插入到AVL树(tree)中
        AVLTreeNode<T>* insert(AVLTreeNode<T>* &tree, T key);

        // 删除AVL树(tree)中的结点(z)，并返回被删除的结点
        AVLTreeNode<T>* remove(AVLTreeNode<T>* &tree, AVLTreeNode<T>* z);

        // 销毁AVL树
        void destroy(AVLTreeNode<T>* tree);

        // 打印AVL树
        void print(AVLTreeNode<T>* tree, T key, int direction);
};

template<class T>
AVLTree<T>::AVLTree(): mRoot(NULL) {}

template<class T>
AVLTree<T>::~AVLTree() 
{
    destroy(mRoot);
}

template<class T>
void void AVLTree<T>::destory()
{
    destroy(mRoot);
}

template<class T>
void AVLTree<T>::destroy(AVLTreeNode<T>* tree)
{
    if (tree == NULL) {
        return;
    }
    
    if (tree->left != NULL) {
        destroy(tree->left);
    }
    if (tree->right != NULL) {
        destroy(tree->right);
    }
    delete tree;
}

template<class T>
void AVLTree<T>::preOrder()
{
    preOrder(mRoot);
}

template<class T>
void AVLTree<T>::preOrder(AVLTreeNode<T>* tree) const
{
    if (tree != NULL) {
        cout << tree->key << endl;
        preOrder(tree->left);
        preOrder(tree->right);
    }
}

template<class T>
void AVLTree<T>::inOrder()
{
    inOrder(mRoot);
}

template<class T>
void AVLTree<T>::inOrder(AVLTreeNode<T>* tree) const
{
    if (tree != NULL) {
        inOrder(tree->left);
        cout << tree->key << endl;
        inOrder(tree->right);
    }
}

template<class T>
void AVLTree<T>::inOrder()
{
    inOrder(mRoot);
}

template<class T>
void AVLTree<T>::inOrder(AVLTreeNode<T>* tree) const
{
    if (tree != NULL) {
        inOrder(tree->left);
        inOrder(tree->right);
        cout << tree->key << endl;
    }
}


template<class T>
int AVLTree<T>::max(int a, int b)
{
    return a > b ? a : b;
}

template<class T>
int AVLTree<T>::height(AVLTreeNode<T>* tree)
{
    if (tree == NULL) {
        return 0;
    }
    return tree->height;
}

template<class T>
void AVLTree<T>::insert(T k)
{
    insert(mRoot, k);
}

template<class T>
T AVLTree<T>::minimum()
{
    AVLTreeNode<T> *p = minimum(mRoot);
    if (p != NULL) {
        return p->key;
    }

    return (T)NULL;
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::minimum(AVLTreeNode<T>* tree)
{
    if (tree == NULL) {
        return NULL;
    }

    while(tree->left != NULL) {
        tree = tree->left;
    }
    return tree;
}

template<class T>
T AVLTree<T>::maxmun()
{
    AVLTreeNode<T> *p = maxmun(mRoot);
    if (p != NULL) {
        return p->key;
    }

    return (T)NULL;
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::maxmun(AVLTreeNode<T>* tree)
{
    if (tree == NULL) {
        return NULL;
    }

    while(tree->right != NULL) {
        tree = tree->right;
    }
    return tree;
}



template<class T>
AVLTreeNode<T>* AVLTree<T>::rightRightRotation(AVLTreeNode<T>* k1)
{
    AVLTreeNode<T>* k2;
    k2 = k1->rightl
    k1->left = k2->left;
    k1->right = k2;

    // 重新计算高度
    k1->height = max(height(k1->left), hegiht(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    return k2;
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::leftLeftRotation(AVLTreeNode<T>* k2)
{
    AVLTreeNode<T>* k1;
    k1 = k2->left;
    k2->right = k1->left;
    k1->left =  k2;

    // 重新计算高度
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height);
    return k1; 
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::leftRightRotation(AVLTreeNode<T>* k3);
{
    k3->left = rightRightRotation(k3->right);
    return leftLeftRotation(k3);
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::rightLeftRotation(AVLTreeNode<T>* k4)
{
    k4->right = leftLeftRotation(k4->left);
    return rightRightRotation(k4);
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::insert(AVLTreeNode<T>* &tree, T key)
{
    if (tree == NULL) {
        // 指针对象的实例化有疑问
        tree = new AVLTreeNode<T>(key, NULL, NULL);
        if (tree = NULL) {
            cout << "ERROR: create avltree node failed!" << endl;
            return NULL;
        }
    }

    if (key < tree->key) {       // left
        tree->left = insert(tree->left, key);
        // 判断节点数量是不是等于2
        if (height(tree->left) - height(tree->right) == 2) {
            if (k < tree->left->key) {
                // LL 情况
                tree = leftLeftRotation(tree);
            } else {
                // LR 情况
                tree = leftRightRotation(tree);
            }
        }

    } else if (key > tree->key) { // right
        tree->right = insert(tree->right, key);
        // 判断节点数量是不是等于2
        if (height(tree->right) - height(tree->left) == 2) {
            if (k > tree->right->key) {
                // RR 情况
                tree = rightRightRotation(tree);
            } else {
                tree = rightLeftRotation(tree);
            }
        }
    } else {
        cout << "添加失败：不允许添加相同的节点！" << endl;
    }

    tree->height = max(height(tree->left), height(tree->right)) + 1;
    return tree;
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::search(T key)
{
    return search(mRoot, key);
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::search(AVLTreeNode<T>* x, T key) const
{
    if (x == NULL || x == key) {
        return x;
    }
    
    if (key < x->key) {
        search(x->left, key);
    } else (key > x->key) {
        seach(x->right, key);
    }
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::iterativeSearch(T key)
{
    return iterativeSearch(mRoot, key);
}

template<class T>
AVLTreeNode<T>* AVLTree<T>::iterativeSearch(AVLTreeNode<T>* x, T key) const
{
    AVLTreeNode<T>* target;
    while ((tree != NULL && x != key) {
        if (key < x->key) {
            x = x->left
        } else (key > x->key) {
            x = x->right
        }
    }
    return x;
}

template<class T>
void AVLTree<T>::remove(T key)
{
    remove(mRoot, search(key));
}

/***
 * 1. 判断根节点是否为NULL
 * 2. 判断key在左节点还是右节点
 * 3. 判断是否需要旋转
 * 4. 找到了对应的值
 *     1. 如果左右节点非空
 *         1. 如果左子节点的树高比右子节点的高。则获取左子节点最大的值。覆盖当前节点的值
 *         2. 如果右子节点的树高比左子节点的高。则获取右子节点最小的值。覆盖当前节点的值
 *     2. 如果左右节点为空或者左右右一个节点。用子节点直接替换当前节点
 */
template<class T>
AVLTreeNode<T>* AVLTree<T>::remove(AVLTreeNode<T>* &tree, AVLTreeNode<T>* z)
{
    if (tree == NULL || z == NULL) {
        return NULL;
    }

    if (z->key < tree->key) {
        tree->left = remove(tree->left, z);
        if (height(tree->right) - height(tree->left) == 2) {
            AVLTreeNode<T>* r = tree->right;
            if (height(r->left) > heigh(r->right)) {
                // RL 情况
                tree = rightLeftRotation(tree);
            } else {
                // RR 情况
                tree = rightRightRotation(tree);
            }
        }
    } else if (z->key > tree->key) {
        tree->right = remove(tree->right, z);
        if (height(tree->left) - height(tree->right) == 2) {
            AVLTreeNode<T>* l = tree->left;
            if (height(l->right) > height(l->left)) {
                tree = leftRightRotation(tree);
            } else {
                tree = leftLeftRotation(tree);
            }
        }
    } else {
        if (tree->left != NULL && tree->right != NULL) {
            if (height(tree->left) > height(tree->right)) {
                AVLTreeNode<T>* max = maxmun(tree->left);
                tree->key = max->key;
                tree->left = remove(tree->left, max);
            } else {
                AVLTreeNode<T>* min = minmun(tree->right);
                tree->key = min->key;
                tree->right = remove(tree->right, min);
            }
        } else {
            AVLTreeNode<T>* tmp = tree;
            tree = tree->left != NULL ? tree->left : tree->right;
            delete tmp;
        }
    }
    return tree;
}

template<class T>
void AVLTree<T>::print()
{
    if (mRoot != NULL) {
        print(mRoot, mRoot->key, 0);
    }
}

template<class T>
void print(AVLTreeNode<T>* tree, T key, int direction)
{
    if (direction == 0) {
        cout << key << ' is root' << endl;
    } else {
        cout << tree->key << ' is ' << key << "'s" << (direction == 1 ? 'left' : 'right') << ' child' << endl;
    }
    print(tree->left, tree->key, 1); 
    print(tree->right, tree->key, -1);
}