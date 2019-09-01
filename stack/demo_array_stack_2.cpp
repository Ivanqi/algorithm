#include <iostream>
#include "demo_array_stack_2.h"
using namespace std;

template<class T>
Stack<T>::Stack()
{
    this->count = 0;
    this->head = new LinkedNode;
    this->head->next = NULL;
}

template <class T>
Stack<T>::~Stack()
{
    LinkedNode *ptr, *temp;
    ptr = head;
    while (ptr->next != NULL) {
        temp = ptr->next;
        ptr->next = temp->next;
        delete temp;
    }
    delete head;
    this->head = NULL;
    this->count = 0;
}

// 入栈
template <class T>
void Stack<T>::push(const T &data) 
{
    LinkedNode *p = new LinkedNode;
    p->data = data;
    p->next = this->head->next;
    head->next = p;
    this->count++;
    cout << "push data: " << this->head->next->data << endl;
}

// 返回栈顶元素，即出栈，不删除栈顶元素
template <class T>
T Stack<T>::peek()
{
    if (this->count == 0 || this->head->next == NULL) {
        cout << "Stack is empty , pop fail" << endl;
        return NULL;
    } else {
        return this->head->next->data;
    }
}

// 返回栈的大小
template <class T>
T Stack<T>::pop()
{
    if (this->count == 0 || this->head->next == NULL) {
        cout << "Stack is empty , pop fail" << endl;
        return NULL;
    } else {
        LinkedNode *temp = this->head->next;
        this->head->next = temp->next;
        T data = temp->data;
        delete temp;
        this->count--;
        return data;
    }
}

// 返回栈大小
template <class T>
int Stack<T>::size() const
{
    return this->count;
}

template <class T>
void Stack<T>::print() const
{
    if (this->head == NULL) {
        cout << "Stack is empty" << endl;
        return;
    }

    LinkedNode *temp = head;
    while(temp != NULL) {
        cout << "print Node val:" << temp->data << endl;
        temp = temp->next;
    }
}

void test_stack() 
{
    Stack <float> s;
    s.push(10.1);
    s.push(20.2);
    s.push(30.);
    s.push(40.4);
    s.push(50.5);
    s.push(60.6);
    s.print();
    s.pop();
    cout << "--------" << endl;
    s.print();
}

int main () {

    test_stack();
    return 0;
}