#include <stdio.h>
#include "main.h"
#include "scheduler.h"
#include "netcom.h"
#include "config.h"
#include "interrupt.h"
#include <signal.h>
#include <unistd.h>

int main() {
    //sigaction(SIGPIPE, &(struct sigaction){SIGPIPE_HANDLER}, NULL);
    //signal(SIGPIPE, SIGPIPE_HANDLER);


    printf("Data Logger initializing...\n");
    printf("Press Ctrl+C to exit\n");
    ConfigRead();
    //SchedulerInit(); //
    NetcomInit();
   
    SchedulerStart();

    printf("\nPress Enter to start data collection ");
    getchar();
    
    
    InterruptCreate();
    collectionFlag = 1;
    for (;;) { } //infinate loop


    return 0;
}