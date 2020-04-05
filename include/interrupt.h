//#define _POSIX_C_SOURCE  199309L
#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <time.h>


timer_t gTimer1;
timer_t gTimer2;

void InterruptCreate(void);
void StartTimer(void);
void StopTimer(void);

#endif