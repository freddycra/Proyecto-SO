#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LEN 100

int validate_cmd(char*); //correct[1] incorrect[0]
void add_history(char*); //agrega al historial de comandos
int excute_cmd(char**);
void split_cmd(char*, char**);

int main(int argc, char** argv){

  int run=1;
  char* buffer = malloc(sizeof(char));
  char** split_buffer = (char**)malloc(5*sizeof(char*));
  while(run){

    printf("$prompt>>> ");
    if(fgets(buffer, MAX_LEN, stdin)!=NULL){ //Lee una linea de consola
      buffer[strcspn(buffer, "\n")] = '\0'; //remplaza el salto de linea de la cadena
      if(validate_cmd(buffer)){
        split_cmd(buffer, split_buffer);
        excute_cmd(split_buffer);
      }
    }

  }


  return 0;
}

int validate_cmd(char* line){
  //Verifica que la linea no este vacia
  return strlen(line)>=1?1:0;
}

void add_history(char* line){
  //TODO lista

}

void split_cmd(char* line, char** split_buffer){
  int i=1;
  char* n = split_buffer[0] = strtok(line, " ");
  while(n!=NULL){
    split_buffer[i++] = n = strtok(NULL, " ");
  }
}

int excute_cmd(char** line){
  pid_t pid = fork();
  if(pid == 0){
    execvp(line[0],line);
  }else{
    wait(NULL);
    printf("Exitoso\n");
  }
}
