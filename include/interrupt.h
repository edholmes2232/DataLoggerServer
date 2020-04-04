//#define _POSIX_C_SOURCE  199309L
#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <time.h>

/*
typedef enum {
    TIMER_SINGLE = 0,
    TIMER_PERIOD
} t_timer;

typedef void (*time_handler)(size_t timerID, void * user_data);

int initialisation();
size_t startTimer(unsigned int interval, time_handler handler,t_timer type, void *user_data);
void stopTimer(size_t timerID);
void finalize();
*/
timer_t gTimer1;
timer_t gTimer2;

void InterruptCreate(void);
void StartTimer(void);
void StopTimer(void);

#endif