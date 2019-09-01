// 类模板的声明，关键字 class 也可以更换成 typename
template<class T>
class Stack {
    public:
        Stack();
        ~Stack();

        void push(const T &data);       // 入栈
        T peek();                       // 返回栈顶元素，即出栈，不删除栈顶元素
        T pop();                        // 出栈，删除栈顶元素
        int size() const;               // 返回栈大小
        void print() const;
    private:
        int count;                      // 存放栈的大小，因为是单链表，所以这里不规定栈的最大可承载量
        struct LinkedNode
        {
            T data;
            LinkedNode *next;
        };
        LinkedNode *head;
};