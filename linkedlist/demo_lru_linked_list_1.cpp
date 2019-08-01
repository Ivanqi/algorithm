#include <iostream>
using namespace std;
/**
 * 思路
 *  1. 维护一个有序单链表，越靠近链表尾部的结点是越早之前访问的。当有一个新的数据被访问时，我们从链表头开始顺序遍历链表
 *  2. 如果此数据之前已经被缓存在链表中了，我们遍历得到这个数据对应的结点，并将其从原来的位置删除，然后再插入到链表的头部
 *  3. 如果此数据没有被缓存在表中，又可以分为两种情况
 *      1. 如果此时缓存没有满，则将此结点直接插入到链表的头部
 *      2. 如果此时缓存已满，则链表尾结点删除，将新的结点插入链表头部
 */

#define MAX 10
class Node {
    public:
        int key;
        Node *next;
    Node(int key = 0, Node* p = NULL) {
        this->key = key;
        this->next = p;
    }
};

class List {
    private:
        Node *head;
        int number;
        void deleteDupliacateNodes(int key);
        int isFull();
        int isEmpty();
    public:
        List () { head =  NULL; number = 0; };
        ~List() {
            delete head;
        };
        void insert(int key);
        Node* search(int key);
        void delkey(int key);
        void reverse();
        void print();
        int getNumber();
        void deleteLast();
};

void List::insert(int key)
{
    if (head == NULL) {
        head =  new Node(key);
        head->next = NULL;
    } else {
        if (isFull()) {
            deleteLast();
        }
        deleteDupliacateNodes(key);
        Node *p = new Node(key);
        p->next = head;
        head = p;
    }
    number++;
}

int List::isFull()
{
    if (number >= MAX) {
        return 1;
    } else {
        return 0;
    }
}

int List::isEmpty()
{
    int zero = 0;
    if (number <= zero || head == NULL) {
        return 1;
    } else {
        return 0;
    }
}

void List::deleteLast()
{
    if (isEmpty()) cout << "没有可删除项" << endl;
    Node *p = head, *q;
    while(p->next != NULL && p->next->next != NULL) {   // 倒数第二个结点
        p = p->next;
    }
    q = p->next;
    p->next = q->next;
    number--;
    delete q;
}

void List::deleteDupliacateNodes(int key)
{
    Node *pos = search(key);
    if (pos) {
        delkey(key);
    }
}

void List::delkey(int key)
{
   if (isEmpty()) cout << "没有可删除项" << endl;
    Node *p = head, *q = head;
    if (p == NULL) {
        cout << "头节点不能为空" << endl;
        return;
    }
    while(p != NULL && p->key != key) {
        q = p;
        p = p->next;
    }
    q->next = p->next;
    number--;
    delete p;
}

Node* List::search(int key) 
{
    Node *cur = head;
    while (cur != NULL) {
        if (cur->key == key) {
            break;
        }
        cur = cur->next;
    }
    return cur;
}

void List::print() 
{
    Node *n = head;
    while (n != NULL) {
        cout << n->key << " ";
        n = n->next;
    }
    cout << endl;
}

int List::getNumber()
{
    return number;
}

int main () {

    List l;
    l.insert(1);
    l.insert(3);
    l.insert(2);
    l.delkey(3);
    l.insert(3);
    l.insert(7);
    l.insert(8);
    l.insert(7);
    l.insert(8);
    l.insert(9);
    l.insert(10);
    l.insert(5);
    l.insert(6);
    l.print();
    // l.deleteLast();
    // l.print();
    
    cout << "链表数量： " << l.getNumber() << endl;;
    return 0;
}


