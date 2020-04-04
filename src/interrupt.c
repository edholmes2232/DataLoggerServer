#include "interrupt.h"
#include "main.h"
#include "scheduler.h"
#include "config.h"
#include <stdio.h>
#include <signal.h>
#include <pthread.h>




timer_t gTimerID;


void StartTimer(void) {
    struct itimerspec value;

    value.it_value.tv_sec = 0; //10s0ns
    value.it_value.tv_nsec = 31250000;//500000000;

    value.it_interval.tv_sec = 0; //and 10 seconds after
    value.it_interval.tv_nsec = 31250000;

    // use realtime clk, send sigev signal when complete
    timer_create(CLOCK_REALTIME, SIGEV_SIGNAL, &gTimerID);
    // arm timer
    timer_settime (gTimerID, 0, &value, NULL);
}
void StopTimer(void) {
    struct itimerspec value;

    value.it_value.tv_sec = 5; //10s0ns
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 5; //and 10 seconds after
    value.it_interval.tv_nsec = 0;

    timer_settime(gTimer1, 0, &value, NULL);
}


void InterruptCreate(void) {
    signal(SIGALRM, IrqHandle);
    StartTimer();

}
/*   
struct timerNode {
    int fd;
    time_handler callback;
    void * user_data;
    unsigned int interval;
    t_timer type;
    struct timer_node * next;
};

static pthread_t gThreadID;
static void * timerThread(void * data);
static struct timer_node *gHead = NULL;
int initialise() {
    if (pthread_create(&gThreadID, NULL, timerThread, NULL)) {
        return 0;
    }
    return 1;
}

size_t startTimer(unsigned int interval, time_handler handler, t_timer type,
                    void * user_data) {
    struct timerNode * newNode = NULL;
    struct itimerspec newVal;

    newNode = (struct timerNode *)malloc(sizeof(struct timerNode));
    
    if (newNode == NULL) return 0;

    newNode->callback = handler;
    newNode->user_data = user_data;
    newNode->interval = interval;
    newNode->type = type;

    newNode->fd = timerfd_create(CLOCK_REALTIME, 0);
    
    if(newNode->fd == -1) {
        free(newNode);
        return 0;
    }

    newVal.it_value.tv_sec = 0;
    newVal.it_value.tv_nsec = interval * 1000000;


    if (type == TIMER_PERIODIC) {
        newVal.it_interval.tv_nsec = interval * 1000000;
    } else {
        newVal.it_interval.tv_nsec = 0;
    }

    newVal.it_interval.tv_sec = 0;

    timerfd_settime(newNode->fd, 0, &newVal, NULL);

    newNode->next = gHead;
    gHead = newNode;

    return (size_t)newNode;
}

void stopTimer(size_t timerID) {
    struct timer_node * tmp = NULL;
    struct timer_node * node = (struct timer_node *)timerID;

    if (node == NULL) return;

    close(node->fd);

    if (node == gHead) {
        gHead = gHead->next;
    } else {
        tmp = gHead;

        while (tmp && tmp->next != node) tmp = tmp->next;

        if (tmp) {
            tmp->next = tmp->next->next;
        }
    }
    if (node) free(node);
}

void finalize() {
    while (gHead) stopTimer((size_t)gHead);

    pthread_cancel(gThreadID);
    pthread_join(gThreadID, NULL);
}

struct timer_node * _get_timer_from_fd(int fd) {
    struct timer_node *tmp = gHead;

    while(tmp) {
        if (tmp->fd == fd) return tmp;

        tmp = tmp->next;
    }
    return NULL;
}

void *_timer_thread(void *data) {
    struct pollfd ufds[

*/
