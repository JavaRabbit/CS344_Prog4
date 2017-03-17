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

  //  Recieve an int for the CLIENT TYPE INT
  //  let encoding, which this is, to be val 10
  int theType;
  recv(sock, &theType, 4,0); // int is size 4
  //printf("server: the type is %d\n", theType);  
  
  // send an INT to the CLIENT
  int myType =10; // let encoding  = 10
  write(sock, &myType, 4); // char is 4

  // if theType, the value got from client is not 10, 
  // we don't want to connect to this client
  if(theType != 10){
    fprintf(stderr, "Wrong type of client trying to connect.");
    close(sock);

  }


  // recieve an int for the Size
  int theSize;
  recv(sock, &theSize, 4, 0); // 4 for int
  //printf("Server.c: the size is %d\n", theSize);


  // receive the plain text
  n = recv(sock, str, theSize,0);  // used to be 6
  //printf("server got str as %s and len is %lu\n", str, strlen(str)); 
 
  // some dummy code
  //write(sock, "foobar", 8);
  
  // receive the key
  p = recv(sock, keyStr,theSize,0);  // used to be 6
  //printf("server got keystr as %s\n", keyStr);
  
 
  // string to hold the cipherstring. length should be str length
  char cipherStr[theSize];
  int i;
  for(i = 0; i < theSize-1; i++){
    int valStr;
    if(str[i] == 32){  // its a space
      valStr = 26;  // set to value after Z
    } else{  // not a space so just -65 
       valStr  = str[i] - 65;  // don't forget if space
       //valStr = valStr %27;
    }
    int valKey;
    if(keyStr[i] == 32){
      valKey = 26;
    } else {
      valKey = keyStr[i] - 65;
      //valKey = valKey%27;
   }
    
    // sum up the message and the key
    int total = valStr + valKey;

    // check if the total is 27. If it is, reassign to 32
    if(total == 26){  // meaning that it is the char after z
      cipherStr[i] = 32;  // meaning that it will code to a space
    } else {
      cipherStr[i] = (total %27) + 65;
    }
   }  // end for loop

  // tack on newline at end of returned string
  cipherStr[theSize-1] = '\n';
  
  write(sock, cipherStr, theSize);
  //printf("after write\n");
}
