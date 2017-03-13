#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <strings.h>

int main(){

 // file descriptors to be used
 int sock, connection_fd;
 
 // struct to hold IP address and port numbers
 struct sockaddr_in server;
 int len;

 //  listen for connections.  
 if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
  perror("socket: ");
 }

 // clear socket
 bzero(&server, sizeof(server));


 // set addressing scheme to AF_INET  (IP)
 server.sin_family = AF_INET;
 
 //  Listen on port 8080
 server.sin_port = htons(8080);  // port number
 
 // Allow any IP to connect
 server.sin_addr.s_addr = htons(INADDR_ANY);

 bind(sock, (struct sockaddr *) &server, sizeof(server));

 // start to listen. Allow at most 5 connections
 //listen(sock, 5);  

 //connection_fd = accept(server, (struct server*) NULL, NULL);


}
