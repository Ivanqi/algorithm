#include <iostream>
#include <assert.h>
#include <vector>
using namespace std;

template<class T>
struct Less {
    bool operator()(const T& left, const T& right) const
    {
        return left < right;
    }
};

template<class T>
struct Greater {
    bool operator()(const T& left, const T& right) const
    {
        return left > right;
    }
};

template<class T, class Compare=Less<T>>
class Heap 
{
    private:
        vector<T> _a;
    public:
        Heap() 
        {
            // 无参的构造函数(系统不会给无参数构造函数)，开始堆是空的不需要做什么事
        } 
        Heap(T* a, size_t n) 
        {
            _a.reserve(n);  // 开空间
            for (size_t i = 0; i < n; ++i) {
                _a.push_back(a[i]);
            }

            // 建堆，找最后一个非叶子结点
            for (int i = (_a.size() - 2) / 2; i >= 0; --i) {   // 不用size_t, 因为i在这可能等于0，用size_t会死循环
                adjustDown(i);
            }
        }

        // 向下调整
        void adjustDown(int i)
        {
            Compare com;
            int parent = i;
            size_t child = parent * 2 + 1;              // 左孩子
            while (child < _a.size()) {
                // 选出小孩子
                if (child + 1 < _a.size() && com(_a[child + 1], _a[child])) {
                    ++child;
                }

                if (com(_a[child], _a[parent])) {
                    swap(_a[child], _a[parent]);       // 交换值
                    parent = child;
                    child = parent * 2 + 1;
                } else {
                    break;
                }
            }
        }

        // 向下调整
        void adjustUp(int child) 
        {
            Compare com;
            int parent = (child - 1) / 2;
            while (parent >= 0) {
                if (com(_a[child], _a[parent])) {
                    swap(_a[parent], _a[child]);
                    child = parent;
                    parent = (child - 1) / 2;
                } else {
                    break;
                }
            }
        }

        // 最后插入
        void push(const T&x)
        {
            _a.push_back(x);
            adjustUp(_a.size() - 1);
        }

        // 删除最大数
        void pop() 
        {
            assert(!_a.empty());
            swap(_a[0], _a[_a.size() - 1]);
            _a.pop_back();
            adjustDown(0);
        }

        // 取顶元素
        T& top()
        {
            assert(!_a.empty());
            return _a[0];
        }

        size_t size()
        {
            return _a.size();
        }

        bool empty()
        {
            return _a.empty();
        }

        void print(const char* prnit_name)
        {
            int i;
            cout << prnit_name << ": ";
            for (i = 0; i < _a.size(); i++) {
                cout << _a[i] << " ";
            }
            cout << endl;
        }
};

void test_maximum_heap() {
    int a[] = {10, 11, 13, 12, 16, 18, 15, 17, 14, 19};
    Heap<int,Greater<int>> hp1(a,sizeof(a)/sizeof(a[0]));
    hp1.push(15);
    hp1.print("最大堆1");
    hp1.pop();
    hp1.print("最大堆2");

}

void test_minimum_heap() {
    int a[] = {10,11,13,12,16,18,15,17,14,19};
    Heap<int,Less<int>> hp1(a,sizeof(a)/sizeof(a[0]));
    hp1.push(55);
    hp1.print("最小堆");
}

void test_minimum_heap_default() {
    int a[] = {10,11,13,12,16,18,15,17,14,19};
    Heap<int> hp1(a,sizeof(a)/sizeof(a[0]));
    hp1.push(88);
    hp1.print("缺省，最小堆");
}

int main () {
    test_maximum_heap();
    printf("\n");
    test_minimum_heap();
    printf("\n");
    test_minimum_heap_default();
    return 0;
    
}