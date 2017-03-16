#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int main(int argc, char *argv[]){

 if( argc !=2){
  fprintf(stderr, "incorrect number of arguments supplied\n");
 }



 // store into variable the size that the user wants
 int sizeWanted = atoi(argv[1]);
 //printf("YOu wanted %d chars\n", sizeWanted);
 
 /*

 
 // Verify that argv[1] is an integer > 0
 if(isdigit(atoi(hello))){
  printf("Yes its a digit\n");
 } 
 */
 // create a string of length argv[1] + 1
 char toReturn[sizeWanted + 1]; 
 int i;
 srand(time(NULL));
 //char randomLetter = 'A' + (rand()%26);
 
 for(i = 0; i < sizeWanted; i++){
  int randNum = rand()%27;
  // to create a char for space, if randNum is 26
  // then make randNum  so that it ends up as 32
  if(randNum == 26){
    randNum = -33;  // so 65 -33 = 32 (which is a space) 
  }

  //  'A' is 65
  char randomLetter = 'A' + randNum;
  toReturn[i] = randomLetter;
 }

 // Spec: the last character should be a new line
 toReturn[sizeWanted] = '\n';
 printf("%s", toReturn);

 //printf("the len of toReturn is %d\n", strlen(toReturn));

}
