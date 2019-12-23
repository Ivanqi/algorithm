#include <iostream>
#include <csignal>
#include <unistd.h>
#include <highTimeSchedule.h>

#define TIME_HEAP_CAP 64
#define TIMESLOT 1
static HighTimeSchedule* schedule = new HighTimeSchedule(TIME_HEAP_CAP);
int g_sec = 0;  // 计数器

// test case1: 1s循环定时器
void do_time1(params *data) {
    printf("timer %s, %d\n", __FUNCTION__, g_sec++);
    schedule->add_timer_by_second(1 * TIMESLOT, do_time1, data);
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

    schedule->add_timer_by_second(2 * TIMESLOT, do_time2, pas);
}

// // test case3: 9s循环定时器
void do_time9(params* data) {
    printf("timer %s, %d\n", __FUNCTION__, g_sec++);
    schedule->add_timer_by_second(9 * TIMESLOT, do_time9, data);
}

void test_do_time9() {

    params* pas = new params;
    char value[] = "do_time9";
    pas->value = value;

    schedule->add_timer_by_second(9, do_time9, pas);
}

// void do_time_date(params* data) {
//     printf("timer %s \n", __FUNCTION__);
// }

// void test_do_time_date(int data) {
//     char date[] = "2019:12:24 5:00:00";
//      params* pas = new params;
//     pas->key = "do_time_date";
//     pas->value = "do_time_date";
//     schedule->add_timer_by_date(date, do_time_date, );
// }

void timer_tick(int sigo) {
    schedule->tick();
}


int main () {

    signal(SIGALRM, timer_tick);
    alarm(TIMESLOT); // 1s的周期心跳


    test_do_time1();


    while(1) pause();
    return 0;
}