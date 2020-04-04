#define _GNU_SOURCE
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
//#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
//#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <time.h>
#include <fcntl.h>
#include "netcom.h"
#include "config.h"
//#include <signal.h>
#include <pthread.h>
// sample rate 32 hz

//movementType *m;
//GLOBALS
fd_set readfds; 
int opt = TRUE; 
int masterSocket, addrLen, newSocket, clientSocket[30],
	maxClients = 2, activity, i, valRead, sd;
int maxSD; 
socklen_t addrSize;
struct sockaddr_in address, newAddr; 	
char buffer[1025]; //data buffer of 1K 
char msg[12][21];

FILE *fp;
int clientReady[25] = {0};
int collectionFlag = 0;
/*j
struct sockDesc {
	int slaveID;
	int sock;
};*/

pid_t childpid;

struct tm *timeNow;
time_t now;

time_t rawtime;
//time (&rawtime);

void SIGPIPE_HANDLER(int x) {
	printf("CLIENT DISCONNECTED!");
}

void NetcomInit() 
{ 
	now = time(NULL);
	timeNow = gmtime(&now);


	//if (DEBUG) printf("Session started at %s\n",ctime(&rawtime));
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < maxClients; i++) 
	{ 
		clientSocket[i] = 0; 
	} 
		
	//create a master socket 
	if( (masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
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
	if (bind(masterSocket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (DEBUG) printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(masterSocket, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrLen = sizeof(address); 
	puts("Waiting for connections ..."); 
	//sigaction(SIGPIPE, &(struct sigaction){SIGPIPE_HANDLER}, NULL);
}
char saveData(int slaveID, char buffer[2048], int rMsg) {
	char fileName[100];
	char dateName[100];
	snprintf(fileName, sizeof(fileName), "Slave-%d_",slaveID);
	strcat(fileName, dateName);
	movementType *m;
	if (rMsg < 1) {
		//printf("E ");

		return 'E';
	} else if (rMsg == 1) {
		//clientReady = 1;
		return 'A';
	} else {
	//snprintf(fileName, sizeof(fileName), "Slave-%d_%s.txt",
	//		slaveID,ctime(&rawtime));
		//snprintf(fileName, sizeof(fileName), "Slave-%d_.txt",slaveID);

		strftime(dateName,sizeof(dateName),
			"%d%b-%H:%M.txt",timeNow);
		fp = fopen(fileName, "a");
	
		printf("Data from #%d:%d\n",slaveID,rMsg);
		//printf("CSEC[9]: %d", m[9].csec);
		m = (movementType *) buffer;
		int ndata = rMsg / sizeof(movementType);

		for (int i=0; i<ndata; i++) {
			//printf("%.2f, %.2f, %.2f\n", c[i].x, c[i].y, c[i].z);
			fprintf(fp,"%.2f,%.2f,%.2f,\t\t%.2f,%.2f,%.2f,\t\t%f,%f,\t\t\t%d:%d:%d:%d\n",
					m[i].ax, m[i].ay, m[i].az, 
					m[i].gx, m[i].gy, m[i].gz, 
					m[i].lat,m[i].lng,
						m[i].hour,m[i].min,m[i].sec,m[i].csec);
		}
		fclose(fp);
		return 'H';
	} 
}



void *socketThread(void *args) {
	//intptr_t nodeSocket = (int) arg;
	printf("Starting a thread!\n");
	char buffer[2048];
	int nodeSocket = ((socketType*)args)->sock;
	int slaveID = ((socketType*)args)->slaveID;
	//free(args);
	printf("\nPOST SLAVE%d Socket %d setup ",slaveID,nodeSocket);
	//int slaveID = args->slaveID;
	int rMsg;
	printf("CLIENT READY ");
	clientReady[slaveID] = 1;
	//printf("nodeSocket:%d ",nodeSocket);
	while(1) {
		//printf("rMsg:%d",rMsg);
		if (collectionFlag) {
			rMsg = recv(nodeSocket, buffer, sizeof(buffer), MSG_DONTWAIT); 
			//printf(" %d ", slaveID);
			//rMsg = recv(6, buffer, sizeof(buffer), 0); 

		//printf("%d's data %d",slaveID,rMsg);
			//printf("Buf:%d",rMsg);
			saveData(slaveID, buffer,rMsg);
		}
		//blocking!
	}
	pthread_exit(NULL);
}





int NetcomSlaveAccept() {
	//clientReady = 0;
	//clear the socket set 
	//static int i = 0;
	if((newSocket = accept(masterSocket, 
				(struct sockaddr *)&address, (socklen_t*)&addrLen))<0){ 
	//if (newSocket = accept(masterSocket, 
	//			(struct sockaddr *)&newAddr, *addrSize) <0){
  //  intptr_t socketFD = accept(masterSocket,
			//	(struct sockaddr*)&address, (socklen_t*)&addrLen))<0){
		perror("ACCEPT");
	}
	pthread_t tid[60];
	printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	int slaveID;
	int sock = newSocket;
	for (int i=0;i<maxClients;i++) {
		if(clientSocket[i] == 0) {
			printf(" clientSock[%d] == %d ",i,newSocket);
			clientSocket[i] = newSocket;
			slaveID = i;
			break;
		}
	}

	
	printf("PRE slave%d socket%d \n",slaveID,newSocket);
	socketType *args= (socketType*)malloc(sizeof(socketType));
	args->slaveID = slaveID;
	args->sock = sock;


	//if ( pthread_create(&tid[i], NULL, socketThread, &newSocket)<0){
    if(pthread_create(&tid[i],NULL,socketThread,args)<0){
		printf("Thread failure\n");
	}
	//collectionFlag = 1;
	//while (!clientReady);
	//collectionFlag = 0;
	while(!clientReady[slaveID])
	return slaveID;
}




	//else its some IO operation on some other socket 
void NetcomUpdate(void) {
	printf("CHECKING SLAVES\n");
	activity = select( maxSD + 1 , &readfds , NULL , NULL , NULL); 

	for (i = 0; i < maxClients; i++) 
	{ 
		sd = clientSocket[i]; 
			
		if (FD_ISSET( sd , &readfds)) 
		{ 
			//Check if it was for closing , and also read the 
			//incoming message 
			if ((valRead = read( sd , buffer, 1024)) == 0) 
			{ 
				//Somebody disconnected , get his details and print 
				getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrLen); 
				printf("Host disconnected , ip %s , port %d \n" , 
					inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
					
				//Close the socket and mark as 0 in list for reuse 
				close( sd ); 
				clientSocket[i] = 0; 
			} 
				
			//Echo back the message that came in 
			else
			{ 
				//set the string terminating NULL byte on the end 
				//of the data read 
				buffer[valRead] = '\0'; 
				//send(sd , buffer , strlen(buffer) , 0 ); 
				printf("NO DISC: %s", buffer);
			} 
		} 
	} 
} 
void NetcomSendMsg(char message, int slaveID) {
	static int count = 0;
	static int rtnv = 0;
	count ++;
	if(DEBUG)
		printf("SEND %c to %d ",message, slaveID);
	//rtnv = write(clientSocket[slaveID], &message, 1);//strlen(message));
	rtnv = send(clientSocket[slaveID], &message, 1, MSG_NOSIGNAL);
	//printf("Write to #%d: %d\n", slaveID,rtnv);
	//printf("%s",msg);
	if (rtnv == -1) {
		perror("SEND PIPE");
		printf("Reconnecting...\n");		
		NetcomReconnect(slaveID);
	}


	if ( rtnv != 1){//strlen(message)) {
		perror("send"); 
	}  
}
int NetcomReconnect(int slaveID) {
	close(clientSocket[slaveID]);
	int newSocket = accept(masterSocket, 
				(struct sockaddr *)&address, (socklen_t*)&addrLen);

	clientSocket[slaveID] = newSocket;
	printf(" SUCCESS\n");
	/*

	sd = clientSocket[slaveID]; 
	activity = select( maxSD + 1 , &readfds , NULL , NULL , NULL); 
	if ((activity < 0) && (errno!=EINTR)) 
		printf("select error"); 
//If something happened on the master socket , 
//then its an incoming connection 
	if (FD_ISSET(masterSocket, &readfds))  {
		if ((newSocket = accept(masterSocket, 
				(struct sockaddr *)&address, (socklen_t*)&addrLen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
		//inform user of socket number - used in send and receive commands 
		if (DEBUG) {
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , 
				newSocket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
		}
		clientSocket[slaveID] = newSocket;
		printf("Reconnected!!!\n");
		return 0;
	}
	return 1;*/
}

char NetcomRecMsg(int slaveID, char buffer[2048], int rMsg) {

	char fileName[100];
	//snprintf(fileName, sizeof(fileName), "Slave-%d_%s.txt",
	//		slaveID,ctime(&rawtime));
	snprintf(fileName, sizeof(fileName), "Slave-%d_.txt",slaveID);

	fp = fopen(fileName, "a");
	movementType *m;
/*
	char* buffer;

	int rMsg;
    buffer = (char *)malloc(pSize*sizeof *buffer);

	movementType *m;
	rMsg = recv(clientSocket[slaveID], buffer, pSize, MSG_DONTWAIT);
	
	

	if (rMsg == 1) {
		//printf("TICK REC\n");
		return buffer[0]; //TICK RECIEVED
	} else*/ if (rMsg < 0) {
		//printf("E ");

		return 'E';
	} else { 
		printf("Data from #%d:%d\n",slaveID,rMsg);
		//printf("CSEC[9]: %d", m[9].csec);
		m = (movementType *) buffer;
		int ndata = rMsg / sizeof(movementType);

		for (int i=0; i<ndata; i++) {
			//printf("%.2f, %.2f, %.2f\n", c[i].x, c[i].y, c[i].z);
			fprintf(fp,"%.2f,%.2f,%.2f,\t\t%.2f,%.2f,%.2f,\t\t%f,%f,\t\t\t%d:%d:%d:%d\n",
					m[i].ax, m[i].ay, m[i].az, 
					m[i].gx, m[i].gy, m[i].gz, 
					m[i].lat,m[i].lng,
						m[i].hour,m[i].min,m[i].sec,m[i].csec);
		}
		fclose(fp);
		return 'H';
	} 
}