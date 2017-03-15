#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]){

 //printf("Hello keygen\n");

 // create a random string of length specified by user
 
 if( argc !=2){
  fprintf(stderr, "incorrect number of arguments supplied\n");
 }


 //printf("%d\n", atoi(argv[1])+3);

 // store into variable the size that the user wants
 int sizeWanted = atoi(argv[1]);
 //printf("YOu wanted %d chars\n", sizeWanted);
 
 /*
 char hello[]= "4";

 printf("another int %d\n", atoi(hello)+10);
 
 // Verify that argv[1] is an integer > 0
 if(isdigit(atoi(hello))){
  printf("Yes its a digit\n");
 } 
 */
 // create a string of length argv[1] + 1
 char toReturn[sizeWanted + 1]; 
 int i;
 for(i = 0; i < sizeWanted; i++){
  toReturn[i] = 'a';
 }

 // Spec: the last character should be a new line
 toReturn[sizeWanted] = '\n';
 printf("%s", toReturn);

 //printf("the len of toReturn is %d\n", strlen(toReturn));

}
