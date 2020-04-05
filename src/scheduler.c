#include "scheduler.h"
#include "netcom.h"
#include "interrupt.h"
#include "config.h"
#include <stdio.h>

char nodeArray[128];
int activeNodes;
int nodeIndex = 0;


void SchedulerStart(void){
    int nodeID; //create array
    printf("Waiting for %d Nodes to connect\n", NUM_NODES);

    activeNodes = 0;
    nodeIndex = 0;
    do {
        nodeID = NetcomNodeAccept();
        printf("Node ID: %d connected,",nodeID);

        if (nodeID != -1) {
            nodeArray[nodeIndex] = nodeID;
            activeNodes++;
            nodeIndex++;
        } 

        printf("\t%d of %d nodes connected\n",activeNodes,NUM_NODES);
    } while (nodeIndex < NUM_NODES);

    //initialize all nodes
    NodeSendTicks('A');
}

void IrqHandle(int sig) { 
    static int i = 1;
    if (i%21 == 0) { //request data after 20
        NodeSendTicks('G');
        //i = 0;
    }
    i++;
}

void NodeSendTicks(char tick) {
    for (int i = 0; i < activeNodes; i++) {
        NetcomSendMsg(tick, i);
    }
}
