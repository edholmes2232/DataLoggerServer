#ifndef _NETCOM_H
#define _NETCOM_H


int funct(int);
void SIGPIPE_HANDLER(int);
int NetcomReconnect(int);
void NetcomInit(void);
int NetcomSlaveAccept(void);
void NetcomUpdate(void);
void NetcomSendMsg(char, int);
char NetcomRecMsg(int, char*,int);
char sendData(int, char*, int);
#define MAX 80 
#define PORT 8080 
#define TRUE 1
#define FALSE 0
#define SA struct sockaddr 

extern int collectionFlag;
typedef struct {
	float x,y,z;
} coordType;

typedef struct {
	double lat,lng;
 	int hour,min,sec,csec;
} gpsData;

typedef struct {
	char* gps;
	char* accel;
} finalData;

typedef struct{
	int slaveID;
	int sock;
} socketType;

typedef struct {
	float ax,ay,az,gx,gy,gz;
	double lat,lng;
	int hour,min,sec,csec;
} movementType;


#endif