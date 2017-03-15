#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){

 int sockfd, n;
 char sendline[1000];
 char recvline[1000];
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

 printf("the plain text count is %d\n", countPlainText);
 
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
 printf("The key char count is %d\n", countKey);
 
 /* If the char count of key is less than
 * the char count of plainText, show error to user
 * and exit with an error and set exit value to 1  */
 if(countKey < countPlainText){
  fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
  exit(1);
 }



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
 
  // ask the user for a line
  printf("Enter Something:\n");
 

  // read string from user
  //fgets(sendline, 1000, stdin); 

  FILE *fp_plain = fopen(argv[1], "r");
  if(fp_plain == NULL){
   printf("No file found\n");
  } else {
   printf("file is found\n");
  }

  /*
  int jj;
  for(jj = 0; ferror(fp_plain) ==0; jj++){
   scanf(fp_plain, "%s", sendline);
  } 
  */
  fscanf(fp_plain, "%s", sendline);
  close(fp_plain); 
  
  printf("the text in plaintext is %s\n", sendline);

  // write send line from sockfd
  write(sockfd, sendline, strlen(sendline)+1);
  
  // read from sockfd the recvline
  read(sockfd, recvline, 1000);

  
  printf("%s", recvline);

// }// end while

}

