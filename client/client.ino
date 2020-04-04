//#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <WiFi101.h>
//#include <sys/socket.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

#include <WiFiNINA.h>
const char *ssid     = "H1S1";
const char *password = "pw1234pw1234";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  delay(5000);

  Serial.println("Connected to wifi");

  struct addrinfo
  {
    int ai_flags;      /* Input flags.  */
    int ai_family;    /* Protocol family for socket.  */
    int ai_socktype;    /* Socket type.  */
    int ai_protocol;    /* Protocol for socket.  */
    //socklen_t ai_addrlen;   /* Length of socket address.  */
    struct sockaddr *ai_addr; /* Socket address for socket.  */
    char *ai_canonname;   /* Canonical name for service location.  */
    struct addrinfo *ai_next; /* Pointer to next in list.  */
  };
}


void func(int sockfd)
{
  char buff[MAX];
  int n;
  for (;;) {
    bzero(buff, sizeof(buff));
    printf("Enter the string : ");
    n = 0;
    while ((buff[n++] = getchar()) != '\n')
      ;
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("From Server : %s", buff);
    if ((strncmp(buff, "exit", 4)) == 0) {
      printf("Client Exit...\n");
      break;
    }
  }
}

int main()
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  // socket create and varification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
    printf("connection with the server failed...\n");
    exit(0);
  }
  else
    printf("connected to the server..\n");

  // function for chat
  func(sockfd);

  // close the socket
  close(sockfd);
}
