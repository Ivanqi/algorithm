#ifndef _HIGH_TIME_SCHEDULE_
#define _HIGH_TIME_SCHEDULE_

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
using namespace std;

class Timer;

struct params {
    void *value;
};

typedef void (*cb_func)(params* pas);

unsigned long long getCurrentMillisecs()
{
    timeval tv;
    ::gettimeofday(&tv, 0);
    unsigned long long ret = tv.tv_sec;
    return ret * 1000 + tv.tv_usec / 1000;
}

class Timer
{   
    friend unsigned long long getCurrentMillisecs();
    public:
        Timer()
        {
            expire = getCurrentMillisecs();
        }

        Timer(int delay)
        {
            expire = getCurrentMillisecs() + delay;
        }
    public:
        unsigned long long expire;              // 定时器生效的确定时间
        void (*cb_func)(params*);               // 定时器回调函数
        params* user_data;                      // 用户数据     
};

class HighTimeSchedule
{
    friend unsigned long long getCurrentMillisecs();
    private:
        Timer** array;
        int capacity;
        int count;
        int first;

        void swap (int i, int j)
        {
            Timer* tmp;
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
        }

        // 自上而下堆化
        void heapify (int n, int parent)
        { 
            int maxPos , left, right;

            while (1) {
                maxPos = parent;
                left = parent * 2;
                right = left + 1;

                if (left <= n && array[parent]->expire > array[left]->expire) maxPos = left;
                if (left <= n && array[parent]->expire > array[right]->expire) maxPos = right;

                if (maxPos == parent) break;
                swap(parent, maxPos);
                parent = maxPos;
            }
        }

        // 扩容，数组容量扩大一倍
        void resize() 
        {
            int new_capacity = 2 * capacity;
            Timer** temp= new Timer* [new_capacity];
            for (int i = 0; i < new_capacity; ++i ) {
                temp[i] = NULL;
            }

            assert(temp != NULL);
            capacity = new_capacity;
            for (int i = 0; i <= count; i++) {
                temp[i] = array[i];
            }

            delete [] array;
            array = temp;
        }

        void add_timer(Timer* timer)
        {
            if (!timer) return;

            if (count >= capacity) resize();

            count++;
            array[count] = timer;
            int i = count;

            while (i / 2 > 0 && array[i]->expire > array[i / 2]->expire) {
                swap(i, i / 2);
                i = i /  2;
            }
        }
        
    public:
        HighTimeSchedule(int cap):  capacity(cap), count(0), first(1)
        {
            array = new Timer* [capacity];
            assert(array != NULL);

            for (int i = 0; i < capacity; ++i) array[i] = NULL;
        }

        ~HighTimeSchedule()
        {
            for (int i = 0; i < capacity; ++i) {
                delete array[i];
            }

            delete [] array;
        }
    public:
        void add_timer_by_millisecond(int millisecond, cb_func func, params* pas)
        {
            assert(millisecond > 0);
            unsigned long long now = getCurrentMillisecs();
            Timer *timer = new Timer(millisecond);
            timer->cb_func = func;
            timer->user_data = pas;

            add_timer(timer);
        }

        void add_timer_by_second(int second, cb_func func, params* pas)
        {
            assert(second > 0);
            unsigned long long now = getCurrentMillisecs();
            Timer *timer = new Timer(second * 1000);
            timer->cb_func = func;
            timer->user_data = pas;

            add_timer(timer);

        }

        void add_timer_by_date(char *date, cb_func func, params* pas)
        {
            struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));

            unsigned long long  datetime = mktime(tmp_time) * 1000;

            Timer *timer = new Timer(datetime);
            timer->cb_func = func;
            timer->user_data = pas;

            add_timer(timer);
            free(tmp_time);
        }

        Timer* top() const
        {
            if (empty()) return NULL;

            return array[first];
        }

        void pop()
        {
            if (empty()) return;

            if (top()) {
                Timer* tmp =  array[first];
                --count;
                if (!empty() && first != count) {
                    swap(first, count);
                    heapify(count, first);
                } else {
                    // delete tmp;
                }
            }
        }

        void tick()
        {
            Timer* tmp = array[first];
            unsigned long long cur = getCurrentMillisecs();

            if (!tmp) return;
            while (!empty() && tmp->expire < cur) {
                if (!tmp) break;
                if (tmp->expire == 0) {
                    pop();
                    tmp = array[first];
                }

                if (tmp->cb_func) {
                    tmp->cb_func(tmp->user_data);
                }
                pop();
                tmp = array[first];
            }
        }

        int empty() const { return count == 0; }
};

#endif