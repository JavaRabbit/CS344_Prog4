#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[]){

 char str[1000];

 // file descriptors to be used
 int sock, connection_fd;
 
 // struct to hold IP address and port numbers
 struct sockaddr_in server;
 int len;
 int portNumber;

 // assign portNumber to argv[1]
 portNumber = atoi(argv[1]);

 //  listen for connections.  
 if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
  perror("socket: ");
 }

 // clear socket
 bzero(&server, sizeof(server));


 // set addressing scheme to AF_INET  (IP)
 server.sin_family = AF_INET;
 
 //  Listen on port specified port
 server.sin_port = htons(portNumber);  // port number
 
 // Allow any IP to connect
 server.sin_addr.s_addr = htons(INADDR_ANY);

 bind(sock, (struct sockaddr *) &server, sizeof(server));

 // start to listen. Allow at most 5 connections
 listen(sock, 5);  

 connection_fd = accept(sock, (struct sockaddr*) NULL, NULL);

 while(1){

  bzero(str,1000);
  read(connection_fd, str, 1000);
  printf("Echo,", str);
  write(connection_fd, str, strlen(str)+1);
 }

}
