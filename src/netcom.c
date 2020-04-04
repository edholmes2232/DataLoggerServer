#define _GNU_SOURCE
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <time.h>
#include <fcntl.h>
#include "netcom.h"
#include "config.h"
#include <pthread.h>

//GLOBALS
int opt = TRUE; 
int masterSocket, addrLen, newSocket, clientSocket[30],
	maxClients = 20;
struct sockaddr_in address, newAddr; 	
char buffer[1025]; //data buffer of 1K 
char msg[12][21];

char fileName[25][30];

FILE *fp;
int clientReady[25] = {0};
int collectionFlag = 0;

pid_t childpid;

struct tm *timeNow;
time_t now;


void NetcomInit()  //Setup Sockets
{ 
	now = time(NULL);
	timeNow = gmtime(&now);
	
	//initialise all client_socket[] to 0 so not checked 
	for (int i = 0; i < maxClients; i++) { 
		clientSocket[i] = 0; 
	} 
		
	//create a master socket 
	if( (masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	

	//type of socket created 
	address.sin_family = AF_INET; 
	//address.sin_addr.s_addr = INADDR_ANY; 
	//address.sin_port = htons( PORT ); 
	address.sin_addr.s_addr = htonl(INADDR_ANY);  //conv from net byte order, use any interfacfe
	address.sin_port = htons(PORT); 

		
	//bind the socket to localhost port 8080
	if (bind(masterSocket, (struct sockaddr *)&address, sizeof(address))<0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (DEBUG) 
		printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(masterSocket, 3) < 0) { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrLen = sizeof(address); 
}

void NetcomSaveData(int nodeID, char buffer[2048], int rMsg) {

	movementType *m;
	if (rMsg > 1) {
		fp = fopen(fileName[nodeID], "a");
		printf("\rData from #%d:%dbits",nodeID,rMsg);
		fflush(stdout);
		m = (movementType *) buffer;
		int ndata = rMsg / sizeof(movementType);

		for (int i=0; i<ndata; i++) {
			fprintf(fp,"%.2f,%.2f,%.2f,\t\t%.2f,%.2f,%.2f,\t\t%f,%f,\t\t\t%d:%d:%d:%d\n",
					m[i].ax, m[i].ay, m[i].az,  //accelerometer
					m[i].gx, m[i].gy, m[i].gz, 	//gyroscope
					m[i].lat,m[i].lng,			//GPS 
					m[i].hour,m[i].min,m[i].sec,m[i].csec); //time
		}
		fclose(fp);
	} 
}


void *socketThread(void *args) {
	char buffer[2048];
	int nodeSocket = ((socketType*)args)->sock;
	int nodeID = ((socketType*)args)->nodeID;
	int rMsg;
	clientReady[nodeID] = 1;
	while(1) {
		if (collectionFlag) {
			rMsg = recv(nodeSocket, buffer, sizeof(buffer), MSG_DONTWAIT); 

			NetcomSaveData(nodeID, buffer,rMsg);
		}
	}
	pthread_exit(NULL);
}



int NetcomNodeAccept() {
	if((newSocket = accept(masterSocket, 
				(struct sockaddr *)&address, (socklen_t*)&addrLen))<0){ 
		perror("ACCEPT");
	}
	pthread_t tid[60];
	if(DEBUG)
		printf("Connection accepted from %s:%d", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	int i,nodeID;
	
	int sock = newSocket;
	for (i = 0;i < maxClients;i++) {
		if(clientSocket[i] == 0) {
			clientSocket[i] = newSocket;
			if (DEBUG)
				printf(" Node #%d on Socket %d\n",i,newSocket);
			nodeID = i;
			break;
		}
	}

	char tmpStr[20];
	strftime(tmpStr,sizeof(tmpStr),
			"%d%b-%H%M.txt",timeNow);
	snprintf(fileName[nodeID], sizeof(fileName[nodeID]),
		"Node-%d-",nodeID);
	strcat(fileName[nodeID], tmpStr);

	socketType *args= (socketType*)malloc(sizeof(socketType));
	args->nodeID = nodeID;
	args->sock = sock;


    if(pthread_create(&tid[i],NULL,socketThread,args)<0){
		perror("threading");
	}
	//collectionFlag = 1;
	//while (!clientReady);
	//collectionFlag = 0;
	while(!clientReady[nodeID])
	return nodeID;
}

 
void NetcomSendMsg(char message, int nodeID) {
	static int count = 0;
	static int rtnv = 0;
	count ++;
	if(DEBUG)
		printf("SEND %c to %d ",message, nodeID);
	rtnv = send(clientSocket[nodeID], &message, 1, MSG_NOSIGNAL);
	if (rtnv == -1) {
		perror("sending error");
	}

	if ( rtnv != 1){
		perror("send"); 
	}  
}