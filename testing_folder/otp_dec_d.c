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

 char str[100000];
 char keyStr[100000];  // string to copy the key


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
   fprintf(stderr, "error on accept\n");
   exit(1);

  }



  //  for each, for off a new process
  pid = fork();
  /*if(pid < 0){
    perror("error on fork");
    exit(1);
  } */


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
  char str[100000];
  char keyStr[100000];
  bzero(str,100000);
  bzero(keyStr,100000);

  // write a char to the client
  char type = 'e'; // e for encryption
  //write(sock, &type, sizeof(char));


  // recieve an int
  int theSize;
  recv(sock, &theSize, 4, 0); // 4 for int
  //printf("the size is %d\n", theSize);


  // receive the plain text
  n = recv(sock, str, theSize,0);  // used to be 6
  //printf("server got str as %s\n", str); 
 
  // some dummy code
  //write(sock, "foobar", 8);
  
  // receive the key
  p = recv(sock, keyStr,theSize,0);  // used to be 6
  //printf("server got keystr as %s\n", keyStr);
  
 
  // string to hold the PLAIN TEXT. length should be str length
  char plainStr[theSize];
  int i;
  // loop only until size -1, because the last char is a newline
  // we don't decipher newlines
  for(i = 0; i < theSize-1; i++){
    int valStr;
    if(str[i] == 32){  // its a space
      valStr = 27;
    } else{  // not a space so just -65 
       valStr  = str[i] - 65;  // don't forget if space
    }
    int valKey;
    if(keyStr[i] == 32){
      valKey = 27;
    } else {
      valKey = keyStr[i] - 65;
   }
    
    // sum up the message and the key
    int total = valStr - valKey;

    // check if the total is 27. If it is, reassign to 32
    if(total == 27){
      plainStr[i] = 32;  // meaning that it will code to a space
    } else {
      plainStr[i] = (total %27) + 65;
    }
   }  // end for loop

  // last char of string should be \n'
  plainStr[theSize-1] = '\n';
  write(sock, plainStr, theSize);
  //printf("after write\n");
}
