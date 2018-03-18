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
int cantidadComandos = 0;

//Metodos de historial
typedef struct{//Crea el nodo
  char* comando;
  struct Nodo * sig;
}Nodo;

Nodo * primero = NULL;
Nodo * ultimo = NULL;

void agregar(Nodo * nodo){
  nodo->sig = NULL;
  if(primero==NULL){
    primero = nodo;
    ultimo = nodo;
  }else{
    ultimo-> sig = nodo;
    ultimo = nodo;
  }
}

void history(){
  int num = 1;
  Nodo * i = primero;
  if(cantidadComandos<=10){
    while(i != NULL){
      printf("%d :%s\n",num,i->comando);
      i=i->sig;
      num++;
    }
  }else{
    int aux = (cantidadComandos - 10);
    while(i != NULL){
      if(num>aux){
        printf("%d :%s\n",num,i->comando);
      }
      i=i->sig;
      num++;
    }
  }
}

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

void add_history(char * line){
  Nodo * nuevoNodo = malloc(sizeof(Nodo));
  nuevoNodo->comando = line;
  agregar(nuevoNodo);
  cantidadComandos++;
}

void split_cmd(char* line, char** split_buffer){
  int i=1;
  char* n = split_buffer[0] = strtok(line, " ");//mete el comando
  while(n!=NULL){
    split_buffer[i++] = n = strtok(NULL, " ");
  }
}

int excute_cmd(char** line){
  pid_t pid = fork();
  if(pid == 0){
    add_history(line[0]);
    printf("%s\n",line[0]);
    execvp(line[0],line);
    if(strcmp(line[0],"history")==0){
      history();
      exit(0);
    }
  }else{
    wait(NULL);
    printf("Exitoso\n");
  }
}
