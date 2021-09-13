#include <signal.h>
#include <time.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

// One shot Timers
// triggers only once

// Periodic Timers
// triggers periodically at regular intervals

// Exponential Back Off timers
// triggers at exponentially places temporal points

// POSIX timers API
// timer_create() create a timer data structure (but do not fire it)
// timer_settime() used to start/stop the timers depending on the arguments
// timer_gettime() return the time remaining for the timer to fire
// timer_delete() delete the timer data structure

//void timer_callback(union sigval arg) {
//    foo(arg.sival_ptr);
//}

static void
print_current_system_time() {
    time_t t;
    time(&t);
    printf("%s ", ctime(&t));
}

typedef struct pair_{
    int a;
    int b;
} pair_t;

pair_t pair = {10, 20};

void
timer_callback(union sigval arg) {
    print_current_system_time();

    pair_t *pair = (pair_t *) arg.sival_ptr;

    printf("pair: [%u %u]\n", pair->a, pair->b);
}

void
timer_demo() {
    int ret;
    struct sigevent evp;
    timer_t timer;
    memset(&timer, 0, sizeof(time_t));
    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_ptr = (void *)&pair;
    evp.sigev_notify_function = timer_callback;
    evp.sigev_notify = SIGEV_THREAD;
    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if (ret < 0) {
        printf("Timer Creation failed, errno = %d\n", errno);
        exit(1);
    }

    struct itimerspec ts;
    ts.it_value.tv_sec = 5;
    ts.it_value.tv_nsec = 0;

    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ret = timer_settime(timer, 0, &ts, NULL);

    if (ret < 0) {
        printf("Timer S failed, errno = %d\n", errno);
        exit(1);
    }else {
        print_current_system_time();
        printf("Timer Alarmed Successfully\n");
    }
};

int
main() {
    timer_demo();
    pause();
    return 0;
}