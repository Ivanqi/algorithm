#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;

int nextPrime(const int n);

template <class T>
class HashTable 
{
    public:
        HashTable(int size = 101);
        int insert(const T& x);
        int remove(const T& x);
        int contains(const T& x);
        void make_empty();
        void display() const;
    private:
        vector<list<T> > lists;
        int currentSize;
        int hash(const string& key);
        int myhash(const T& x);
        void rehash();
};

template <class T>
HashTable<T>::HashTable(int size)
{
    lists = vector<list<T> >(size);
    currentSize = 0;
}

template <class T>

/**
 * 1. 借助Horner's 规则，构造一个质数（通常是37）的多项式，（非常的巧妙，不知道为什么是37）
 * 2. 计算公式为: key[keysize-i-1]*37i, 0<=i<keysize求和
 * 3. 该方法存在的问题是如果字符串关键字比较长，散列函数的计算过程就变长，有可能导致计算的hashVal溢出。针对这种情况可以采取字符串的部分字符进行计算，例如计算偶数或者奇数位
*/
int HashTable<T>::hash(const string& key)
{
    int hashVal = 0;
    int tableSize = lists.size();
    for (int i = 0; i < key.length(); i++) {
        hashVal = 37 * hashVal + key[i];
    }
    hashVal %= tableSize;
    if (hashVal < 0) {
        hashVal += tableSize;
    }
    return hashVal;
}

template <class T>
int HashTable<T>::myhash(const T& x)
{
    string key = x.getName();
    return hash(key);
}

template <class T>
int HashTable<T>::insert(const T& x)
{
    list<T> &whichlist = lists[myhash(x)];
    // find 函数主要实现的是在容器内查找指定元素，并且这个元素必须是基础数据类型的。查找成功返回一个指定元素的迭代器，查找失败返回end迭代器
    if (find(whichlist.begin(), whichlist.end(), x) != whichlist.end()) {
        return 0;
    }

    whichlist.push_back(x);
    currentSize = currentSize + 1;
    if (currentSize > lists.size()) {
        rehash();
    }
    return 1;
}

template <class T>
int HashTable<T>::remove(const T& x) 
{
    typename std::list<T>::iterator iter;
    list<T> & whichlist = lists[myhash(x)];
    iter = find(whichlist.begin(), whichlist.end(), x);
    if (iter != whichlist.end()) {
        whichlist.erase(iter);
        currentSize--;
        return 1;
    }
    return 0;
}

template <class T>
int HashTable<T>::contains(const T& x)
{
    list<T>  whichlist;
    typename std::list<T>::iterator iter;
    whichlist = lists[myhash(x)];
    iter = find(whichlist.begin(), whichlist.end(), x);
    if (iter != whichlist.end()) {
        return 1;
    }
    return 0;
}

template <class T>
void HashTable<T>::make_empty()
{
    for (int i = 0; i < lists.size(); i++) {
        lists[i].clear();
    }
    currentSize = 0;
    return 0;
}


template <class T>
void HashTable<T>::rehash()
{
    vector<list<T> > oldLists = lists;
    lists.resize(nextPrime(2 * lists.size()));
    for (int i = 0; i < lists.size(); i++) {
        lists[i].clear();
    }
    currentSize = 0;

    for (int i = 0; i < oldLists.size(); i++) {
        typename std::list<T>::iterator iter = oldLists[i].begin();
        while(iter != oldLists[i].end()) {
            insert(*iter++);
        }
    }
}

template <class T>
void HashTable<T>::display()const
{
    for (int i = 0; i < lists.size(); i++) {
        cout << i <<": ";
        typename std::list<T>::const_iterator iter = lists[i].begin();
        while(iter != lists[i].end()) {
            cout << *iter << " ";
            ++iter;
        }
        cout << endl;
    }
}

int nextPrime(const int n)
{
    int ret, i;
    ret = n;
    while(1) {
        int flag = 1;
        for (i = 2; i < sqrt(ret); i++) {
            if (ret % i == 0) {
                flag = 0;
                break;
            }
        }
        if (flag == 1) {
            break;
        } else {
            ret = ret + 1;
            continue;
        }
    }
    return ret;
}


class Employee
{
    public:
        Employee() {}
        Employee(const string n, int s=0):name(n),salary(s) {}
        const string & getName() const { return name; }

        bool operator == (const Employee &rhs) const
        {
            return getName() == rhs.getName();
        }

        bool operator != (const Employee &rhs) const
        {
            return !(*this == rhs);
        }

        friend ostream& operator <<(ostream& out, const Employee& e)
        {
            out << "(" << e.name << "," << e.salary << ") ";
            return out;
        }
    private:
        string name;
        int salary;
};


int main ()
{
    Employee e1("Tom", 6000);
    Employee e2("Anker", 7000);
    Employee e3("Jermey", 8000);
    Employee e4("Lucy", 7500);
    HashTable<Employee> emp_table(13);

    emp_table.insert(e1);
    emp_table.insert(e2);
    emp_table.insert(e3);
    emp_table.insert(e4);

    cout << "Hash table is: " << endl;
    emp_table.display();

    if (emp_table.contains(e4) == 1) {
        cout << "Tom is exists in hash table" << endl;
    }

    if (emp_table.remove(e1) == 1) {
        cout << "Removing Tom from the hash table successfully" << endl;
    }

    if (emp_table.contains(e1) == 1) {
        cout << "Tom is exist in hash table " << endl;
    } else {
        cout << "Tom is not exist in hash table " << endl;
    }
    exit(0);
}