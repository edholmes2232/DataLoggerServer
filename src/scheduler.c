#include "scheduler.h"
#include "netcom.h"
#include "interrupt.h"
#include "config.h"
#include <stdio.h>

#define PKT_SIZE 2400
char currentSlaveID[128];
char slaveArray[128];
int activeSlaves;

int slaveAcks[128] = {0};


int slaveIndex = 0;

void SchedulerStart(void){
    //tByte activeSlaves;
    int slaveID; //create array
    printf("Waiting for %d Nodes to connect\n", NUM_SLAVES);

    activeSlaves = 0;
    slaveIndex = 0;
    do {
        slaveID = NetcomSlaveAccept();
        printf("Slave ID: %d connected",slaveID);


        if (slaveID != -1) {
            slaveArray[slaveIndex] = slaveID;
            activeSlaves++;
            slaveIndex++;
        } 

        printf("\t%d of %d slaves connected\n",activeSlaves,NUM_SLAVES);
    } while (slaveIndex < NUM_SLAVES);

    //init all slaves
    SlaveSendTicks('A');

    //InterruptCreate();
}

void IrqHandle(int sig) { 
    static int i = 1;
    if (i%21 == 0) { //request data after 20
    /*
        for (int j = 0; j < activeSlaves; j++) {
            if (slaveAcks[j] > 0) {
                printf("%d acks missed from slave %d\n",
                        slaveAcks[j], j);
            }
            if (slaveAcks[j] > 15) {
                printf("Slave %d most likely disconnected\n", j);
                printf("Attempt rec\n");
                if (NetcomReconnect(j) == 0) {
                    slaveAcks[j] = 0;
                }
            }
            slaveAcks[j]++;
        } */
        SlaveSendTicks('G');
        //SlaveRecData();
        i = 0;
    } else if (i%5 == 0) {
        //SlaveRecData(); //try recieve data every 5
    }
    i++;
}

void SlaveSendTicks(char tick) {
    //printf("Sending %c\n",tick);
    for (int i = 0; i < activeSlaves; i++) {
        NetcomSendMsg(tick, i);
    }
}
/*
void SlaveRecData() {
    static char rtnv;
    for (int i = 0; i < activeSlaves; i++) {
        if (slaveAcks[i] < 16) {
            rtnv = NetcomRecMsg(i, PKT_SIZE*(slaveAcks[i]+1));
        } else {
            rtnv = NetcomRecMsg(i, PKT_SIZE); //Stop buffer growing after disconnect
        }
        if (rtnv == 'H') slaveAcks[i] = 0;
    }
}
*/

void SchedulerUpdate(void) {

    SlaveSendTicks('C');
    //SlaveProcessAcks();
}
