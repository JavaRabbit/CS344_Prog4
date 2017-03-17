#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// struct to help store size
struct myPacket{
 int theSize;

};
int main(int argc, char **argv){

 int sockfd, n;
 char sendline[1000];
 char recvline[1000];
 char sendKey[1000];
 struct sockaddr_in servaddr;

 // get portNumber from argv at index 3
 int portNumber = atoi(argv[3]);


 // open and check plaintext file and key File
 // Do character validaton
 FILE *plainText = fopen(argv[1], "r");
 if(plainText == NULL){
  printf("no such file \n");
 }


 // check that the key file is valid
 FILE *keyFile = fopen(argv[2], "r");
 if(keyFile == NULL){
  printf("no such key file\n");
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

// create a struct and assign the size
struct myPacket msg;
msg.theSize = countPlainText;
printf("the msg size is %d\n", msg.theSize);


 sockfd=socket(AF_INET, SOCK_STREAM, 0);
 bzero(&servaddr, sizeof servaddr);

 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(portNumber);

 inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

 connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

 //while(1){
  // clear send and recieve lines
  bzero(sendline, 1000);
  bzero(recvline, 1000);
  bzero(sendKey, 1000);
 
  
// read string from user
  //fgets(sendline, 1000, stdin); 

  FILE *fp_plain = fopen(argv[1], "r");
  if(fp_plain == NULL){
   fprintf(stderr, "No file found\n");
   exit(1);
  } else {
   printf("file is found\n");
  }

  //  copy the plainText string into sendLine 
  //fscanf(fp_plain, "%s", sendline);
  
  fgets(sendline, 7000, fp_plain) != NULL;
  close(fp_plain); 
  
  printf("the text in plaintext is %s\n", sendline);


  /* Read the key  */
  FILE *fp_key = fopen(argv[2], "r");
  if(fp_key == NULL){
    fprintf(stderr, "no key found\n");
  } else {
    //printf("file found\n");
 }

 //fscanf(fp_key, "%s", sendKey);
 // try fgets 
 fgets(sendKey, 7000, fp_key) != NULL;
 printf("the key is %s and the len is %lu\n", sendKey, strlen(sendKey));
 close(fp_key);

 // check if the type is 'd' for decryption
 char type = 'd';
 //recv(sockfd, &type, sizeof(char), 0);
 //printf("the type is %c\n", type);
  if(type == 'd'){ 
   //printf("yes, I can decrypt\n");
  } else {
   fprintf(stderr, "Connect connect to Encryption server");
   close(sockfd);

 }


  // try to send an int to the server
  send(sockfd, &(msg.theSize), sizeof(msg.theSize),0);


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
  read(sockfd, recvline, msg.theSize); // used to be 6


  printf("%s\n", recvline);

}

