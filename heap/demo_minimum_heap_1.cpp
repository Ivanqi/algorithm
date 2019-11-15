#include <assert.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

const int DefaultSize = 50;

template<class T>
class PQueue 
{
    protected:
        T* pqelements;
        int num;
        int maxSize;
        void Adjust() 
        {
            // 将队尾元素按其优先权调整到适当的位置，保持所有元素优先权从小到大有序
            T temp = pqelements[num - 1];
            for (int j = num - 2; j > 0; j--) {
                if (pqelements[j] <= temp){
                    break;
                } else {
                    pqelements[j + 1] = pqelements[j];
                }
                pqelements[j] = temp;
            }
        }
    public:
        // Default constructor
        PQueue(int sz = DefaultSize):maxSize(sz), num(0)
        {
            // 创建队列空间
            pqelements = new T[maxSize];
            // 断言，动态存储分配是否成功
            assert(NULL != pqelements);
        }

        // Default destructor
        ~PQueue()
        {
            delete [] pqelements;
        }

        bool Insert(const T& x)
        {
            if (num == maxSize) {
                return false;
            }

            pqelements[num++] = x;
            Adjust();
        }

        bool RemoveMin(T& x)
        {
            if (0 == num) return false;

            x = pqelements[0];
            for (int i = 1; i < num; i++) {
                pqelements[i - 1] = pqelements[i];
            }
            num--;
            return true;
        }

        bool GetFront(T& x)
        {
            if (0 == num) {
                return false;
            } else {
                x = pqelements[0];
                return true;
            }
        }

        void MakeEmpty()
        {
            num = 0;
        }

        bool IsEmpty() const
        {
            return num == 0 ? true : false;
        }

        bool IsFull() const
        {
            return num == maxSize ? true : false;
        }

        int getSize() const
        {
            return num;
        }
};

template<class T>
class MinHeap:public PQueue<T>
{
    public:
        // Default constructor
        MinHeap(int sz = DefaultSize)
        {
            maxHeapsize = DefaultSize < sz ? sz : DefaultSize;
            heap = new T[maxHeapsize];
            if (NULL == heap) {
                cerr << "堆存储空间分配失败！" << endl;
                exit(1);
            }
            currentSize = 0;
        }

        MinHeap(T arra[], int n)
        {
            maxHeapsize = DefaultSize < n ? n : DefaultSize;
            heap = new T[maxHeapsize];

            if (NULL == heap) {
                cerr << "堆存储空间分配失败" << endl;
                exit(1);
            }

            // 复制堆数组，建立当前大小
            for (int i = 0; i < n; i++) {
                heap[i] = arra[i];
            }
            currentSize = n;
            // 找最初调整位置
            int currentPosition = (currentSize - 2) / 2;
            while (currentPosition >= 0) {
                // 局部自上而下调整
                shiftDown(currentPosition, currentSize - 1);
                // 换下一个分支调整
                currentPosition--;
            }
        }

        // Default destructor
        ~MinHeap()
        {
            delete []heap;
        }

        bool Insert(const T& x)
        {
            if (currentSize == maxHeapsize) {
                cerr << "Heap Full" << endl;
                return false;
            }
            heap[currentSize] = x;
            shiftUp(currentSize);
            currentSize++;
            return true;
        }

        bool RemoveMin(T& x)
        {
            if (!currentSize) {
                cerr << "Heap empty!" << endl;
                return false;
            }

            x = heap[0];
            heap[0] = heap[currentSize - 1];
            currentSize--;
            shiftDown(0, currentSize);
            return true;
        }

        // 判断是否为空
        bool IsEmpty() const
        {
            return currentSize == 0 ? true : false;
        }

        bool IsFull() const
        {
            return currentSize == 0 ? true : false;
        }

        void MakeEmpty()
        {
            currentSize = 0;
        }

        int GetSize()
        {
            return currentSize;
        }
    private:
        // 存放最小堆中元素的数组
        T *heap;
        int currentSize;
        int maxHeapsize;
        void shiftDown(int start, int m)
        {
            // 从节点start开始m为止，自上而下，如果子女的值小于父节点的值
            // 则关键码小的上浮，继续向下比较，就可以将一个局部调整为最小堆
            int i = start;
            int j = 2 * i + 1;  // j 是 i的做子女的位置
            T temp = heap[i];

            while (j < m) {
                if (j < m && heap[j] > heap[j + 1]) {
                    // 让j指向两子女中较小者
                    j++;
                }

                if (temp <= heap[j]) {
                    break;
                } else {
                    heap[i] = heap[j];
                    i = j;
                    j = 2 * j + 1;
                }
            }
            heap[i] = temp;
        }

        void shiftUp(int start)
        {
            // 从节点start开始到节点0为止，自下向上比较，如果子女的值小于父节点的值
            // 则互相交换，将集合从新调整为最小堆
            int j = start;
            int i = (j - 1) / 2;
            T temp = heap[j];

            while (j > 0) {
                // 父节点的值小，则不调整
                if (heap[i] <= temp) {
                    break;
                } else {
                    // 父节点饿值大，则调整
                    heap[j] = heap[i];
                    j = i;
                    i = (i - 1) / 2;
                }
            }
            heap[j] = temp;
         }
};

int main() {
    MinHeap<int> * heap = new MinHeap<int >(8);
    heap->Insert(53);
    heap->Insert(17);
    heap->Insert(78);
    heap->Insert(9);
    heap->Insert(45);
    heap->Insert(65);
    heap->Insert(87);
    heap->Insert(23);

    cout<<"依次删除最小堆中的元素<从小到大>：\n"<<endl;
    int value;
    heap->RemoveMin(value);
    cout<<value<<" ";

    heap->RemoveMin(value);
    cout<<value<<" ";

    heap->RemoveMin(value);
    cout<<value<<" ";

    heap->RemoveMin(value);
    cout<<value<<" ";

    heap->RemoveMin(value);
    cout<<value<<" ";


    heap->RemoveMin(value);
    cout<<value<<" ";

    heap->RemoveMin(value);
    cout<<value<<" ";

    heap->RemoveMin(value);
    cout<<value<<" \n\n";
    delete heap;
    return 0;
}

