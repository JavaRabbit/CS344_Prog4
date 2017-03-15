#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){

 int sockfd, n;
 char sendline[1000];
 char recvline[1000];
 struct sockaddr_in servaddr;

 sockfd=socket(AF_INET, SOCK_STREAM, 0);
 bzero(&servaddr, sizeof servaddr);

 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(22000);

 inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

 connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

 while(1){
  bzero(sendline, 1000);
  bzero(recvline, 1000);
  fgets(sendline, 1000, stdin); 
  
  write(sockfd, sendline, strlen(sendline)+1);
  read(sockfd, recvline, 1000);
  printf("%s", recvline);

 }// end while

}

