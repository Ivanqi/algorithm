#ifndef _HIGH_TIME_SCHEDULE_
#define _HIGH_TIME_SCHEDULE_

#include <iostream>
#include <time.h>
#include <assert.h>

class Timer;

struct params {
    void *value;
};

typedef void (*cb_func)(params* pas);


class Timer
{
    public:
        time_t expire;              // 定时器生效的确定时间
        void (*cb_func)(params*);     // 定时器回调函数
        params* user_data;          // 用户数据     
};

class HighTimeSchedule
{
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
            int parent = i / 2;

            while (parent > 0 && array[i]->expire > array[parent]->expire) {
                swap(i, parent);
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
        void add_timer_by_second(int second, cb_func func, params* pas)
        {
            assert(second > 0);
            time_t now = time(NULL);
            Timer *timer = new Timer;
            timer->expire = now + second;
            timer->cb_func = func;
            timer->user_data = pas;

            add_timer(timer);

        }

        void add_timer_by_date(char *date, cb_func func, params* pas)
        {
            struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));
            strptime(date, "%Y-%m-%d%H:%M:%S", tmp_time);

            time_t datetime = mktime(tmp_time); 
            time_t now = time(NULL);
            int second = datetime - now;

            assert(second <= 0);

            Timer *timer = new Timer;
            timer->expire = second;
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

            if (array[first]) {
                delete array[first];
                array[first] == array[count];
                --count;
                heapify(count, first);
            }
        }

        void tick()
        {
            Timer* tmp = array[first];
            time_t cur = time(NULL);

            while (!empty()) {
                if (!tmp) {
                    // printf("tmp: %d", tmp->expire);
                    break;
                }

                if (cur > tmp->expire) {
                    // printf("过时了:%d\n" , tmp->expire);
                    break;
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