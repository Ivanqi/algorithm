#include <iostream>
#include <highTimeSchedule.h>
#include <chrono>
#include <thread>
#include <sys/time.h>


using namespace std;

#define TIME_HEAP_CAP 64
#define TIMESLOT 1
static HighTimeSchedule* schedule = new HighTimeSchedule(TIME_HEAP_CAP);
int g_sec = 0;  // 计数器

// test case1: 1s循环定时器
void do_time1(params *data) {
    printf("timer %s, %d\n", __FUNCTION__, g_sec++);
    schedule->add_timer_by_second(1, do_time1, data);
}

void test_do_time1() {

    params* pas = new params;
    char value[] = "do_time1";
    pas->value = value;
    
    schedule->add_timer_by_second(1, do_time1, pas);
}

// test case2: 2s单次定时器
void do_time2(params* data) {
    printf("timer %s, %d\n", __FUNCTION__, g_sec++);
}
void test_do_time2() {
    params* pas = new params;
    char value[] = "do_time1";
    pas->value = value;

    schedule->add_timer_by_second(2, do_time2, pas);
}

// // test case3: 9s循环定时器
void do_time9(params* data) {
    printf("timer %s, %d\n", __FUNCTION__, g_sec++);
    schedule->add_timer_by_second(9, do_time9, data);
}

void test_do_time9() {

    params* pas = new params;
    char value[] = "do_time9";
    pas->value = value;

    schedule->add_timer_by_second(9, do_time9, pas);
}

void do_time_date(params* data) {
    printf("timer %s, %d \n", __FUNCTION__, g_sec++);
}

void test_do_time_date() {
    char date[5][20] = {"2019-12-25 21:53:00", "2019-12-25 21:54:00", "2019-12-25 21:55:00", "2019-12-25 21:56:00", "2019-12-25 21:57:00"};
    params* pas = new params;
    char value[] = "test_do_time_date";
    pas->value = value;

    for (int j = 0; j < 5; j++ ) {
        schedule->add_timer_by_date(date[j], do_time_date, pas);
    }
}

int main () {

    test_do_time9();

    while (true) {
        schedule->tick();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    return 0;
}