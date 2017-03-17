#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// struct to help store size
struct myPacket {
 int theSize;
 int theType; 
};



int main(int argc, char **argv){

 int sockfd, n;
 char sendline[100000];
 char recvline[100000];
 char sendKey[100000];
 struct sockaddr_in servaddr;

 // get portNumber from argv at index 3
 int portNumber = atoi(argv[3]);


 // open and check plaintext file and key File
 // Do character validaton
 FILE *plainText = fopen(argv[1], "r");
 if(plainText == NULL){
  fprintf(stderr,"no such file \n");
  exit(1);
 }


 // check that the key file is valid
 FILE *keyFile = fopen(argv[2], "r");
 if(keyFile == NULL){
  fprintf(stderr,"no such key file\n");
  exit(1);
 }

 // compare char count for each file
 int countPlainText = 0;
 int countKey = 0;  // initialize to 0

 /*  character or EOF flag */
 int ch;
 
 while(1){
  ch = fgetc(plainText);
  /* If EOF is detected, break*/
  if(ch == EOF){
   break;
  }
  // otherwise add to count
  ++countPlainText;
 }

 //printf("the plain text count is %d\n", countPlainText);
 
 /* Count the number of chars in the key file*/
 int keyCounter; 
 while(1){
  keyCounter = fgetc(keyFile);
  /* If EOF is detected, break */
  if(keyCounter == EOF){
   break;
  }
  // otherwise add to the count
  ++countKey;
 }
 //printf("The key char count is %d\n", countKey);
 
 /* If the char count of key is less than
 * the char count of plainText, show error to user
 * and exit with an error and set exit value to 1  */
 if(countKey < countPlainText){
  fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
  exit(1);
 }

 // else create a struct and assign the size
 struct myPacket msg;
 msg.theSize = countPlainText;
 //printf("the msg size is %d\n", msg.theSize);

 sockfd=socket(AF_INET, SOCK_STREAM, 0);
 bzero(&servaddr, sizeof servaddr);

 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(portNumber);

 inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

 // connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));


 //while(1){
  // clear send and recieve lines
  bzero(sendline, 100000);
  bzero(recvline, 100000);
  bzero(sendKey, 100000);
 
  
// read string from user
  //fgets(sendline, 1000, stdin); 

  FILE *fp_plain = fopen(argv[1], "r");
  if(fp_plain == NULL){
   fprintf(stderr, "No file found\n");
  } else {
   //printf("file is found\n");
  }

  //  copy the plainText string into sendLine 
  //fscanf(fp_plain, "%s", sendline);
  
  fgets(sendline, 100000, fp_plain) != NULL;
  close(fp_plain); 
  
  //printf("Cli: the text in plaintext is %s and len is %lu\n", sendline, strlen(sendline));

  // Iterate through sendline to check for bad characters
  int iter;
  // valid characters are value 65-90 inclusive (A-Z), and 32 which is the space
  for(iter = 0; iter < msg.theSize-1; iter++){    // use theSize -1 since don't check '\n'
    //  if the value is not between 65 and 90 inclusive, or is not equal to 32
    //  then it is a bad character. set exit value to 1 and show error to user
    //printf("gets here to check loop\n");
    if(sendline[iter] < 65 || sendline[iter] > 90){  // it is <A or > Z
      if(sendline[iter] == 32) {  // its a space, so its not bad
        continue; // just continue
      }
      // else we've found a bad character.
      fprintf(stderr, "otd_enc_d error: Input Contains BAD Characters\n");
      exit(1);
    }
  } // end of for(iter.. loop to check for bad values

  connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  /* Read the key  */
  FILE *fp_key = fopen(argv[2], "r");
  if(fp_key == NULL){
    printf("no key found\n");
  } else {
    //printf("file found\n");
 }

 //fscanf(fp_key, "%s", sendKey);
 // try fgets  to get the key into sendKey
 fgets(sendKey, 100000, fp_key) != NULL;
 //pirintf("the key is %s and the len is %lu\n", sendKey, strlen(sendKey));
 close(fp_key);

  // try to send an int of 10 (because we want encryption" to server.c
  msg.theType = 10;  
  send(sockfd, &(msg.theType), 4, 0); // int is size 4
 
 // read the INT that server sent back
  int intFromServer;
  read(sockfd, &intFromServer, 4); // size of int is 4
  //printf("client here: got back %d from server.\n", intFromServer);
  
   
  // if a wrong type of server is contacted, close connection
  if(intFromServer != 10){
    fprintf(stderr, "Cannot connect to a Decryption Server.\n");
    close(sockfd);
   }

  // try to send an int to the server
  send(sockfd, &(msg.theSize), sizeof(msg.theSize), 0);

  // write send line from sockfd
  //write(sockfd, sendline, strlen(sendline)+1);
  
  // try  send  the plain text
  send(sockfd, sendline, strlen(sendline) ,0);  


  //  try a receive  for dummy code
  char foo[440];
  //recv(sockfd, foo, 10,0);
  
  //  write send the key
  //write(sockfd, sendKey, strlen(sendKey)+1);
  send(sockfd, sendKey, strlen(sendKey), 0);
   
   // read from sockfd the recvline
  read(sockfd, recvline, msg.theSize);  //used to be 6
  
  //printf("HSC:the length of recvline is %lu\n", strlen(recvline));
  //printf("HSC: %s\n", recvline);
  printf("%s", recvline);


}

