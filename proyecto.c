#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){

  char line[80];
  int run=1;

  while(run){
    printf("$prompt> ");
    fgets(line,80,stdin);
  }


  return 0;
}
