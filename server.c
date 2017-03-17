#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>

// prototypes
void doprocessing(int sock);

int main(int argc, char *argv[]){

 char str[1000];
 char keyStr[1000];  // string to copy the key


 // file descriptors to be used
 int sock_fd, newsockfd, connection_fd, client;

 // struct to hold IP address and port numbers
 struct sockaddr_in server, client_addr;
 int len;
 int portNumber;
 int pid;

 // assign portNumber to argv[1]
 portNumber = atoi(argv[1]);

 //  Call to socket function
 if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
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

 // Bind the host address
 bind(sock_fd, (struct sockaddr *) &server, sizeof(server));

 // start to listen. Allow at most 5 connections
 listen(sock_fd, 5);

 client = sizeof(client_addr);

 while(1){
  connection_fd = accept(sock_fd, (struct sockaddr*) &client_addr, &client);

  if(connection_fd < 0){
   perror("error on accept\n");
   exit(1);

  }



  //  for each, for off a new process
  pid = fork();
  /*if(pid < 0){
    perror("error on fork");
    exit(1);
  } */

  printf("gets into fork");

  /* This is the child process*/
  if(pid ==0){
   close(sock_fd);
   doprocessing(connection_fd);
   exit(0);

  } else{
   close(connection_fd);
  }

 } // end of while

} // end of main

void doprocessing(int sock){
  int n,p;
  char str[1000];
  char keyStr[1000];
  bzero(str,1000);
  bzero(keyStr,1000);

  n = read(sock, str, 1000);
  //p = read(sock, keyStr,1000);

  printf("get here after read\n");
  
 
  // string to hold the cipher. length should be str length
  char cipherStr[strlen(str)+1];
  int i;
  for(i = 0; i < strlen(str); i++){
    int valStr;
    if(str[i] == 32){  // its a space
      valStr = 27;
    } else{  // not a space so just -65 
       valStr  = str[i] - 65;  // don't forget if space
       valStr = valStr %27;
    }
    int valKey;
    if(keyStr[i] == 32){
      valKey = 27;
    } else {
      valKey = keyStr[i] - 65;
      valKey = valKey%27;
   }
    
    // sum up the message and the key
    int total = valStr + valKey;

    // check if the total is 27. If it is, reassign to 32
    if(total == 27){
      cipherStr[i] = 32;  // meaning that it will code to a space
    } else {
      cipherStr[i] = (total %27) + 65;
    }
   }  // end for loop

  
  write(sock, str, strlen(str)+1);
  //printf("after write\n");
}
