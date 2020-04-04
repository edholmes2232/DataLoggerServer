#ifndef _SCH_H
#define _SCH_H

//#include "main.h"

void SchedulerInit(void);
void SchedulerStart(void);
void SchedulerUpdate(void);
void SchedulerDispatch(void);
void bufferCollect(int);
void IrqHandle(int);
void SlaveSendTicks(char);

void SlaveRecData(void);

#endif