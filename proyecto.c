#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#define MAX_LEN 100

int validate_cmd(char*); //correct[1] incorrect[0]
void add_history(char*); //agrega al historial de comandos
void excute_cmd(char**);
int split_cmd(char*, char**);
void extraer(char*);
int run = 1;
int cantidad = 0;

//Metodos de lista
struct Nodo{//Crea el nodo
  char comando[MAX_LEN];
  int posicion;
  struct Nodo * sig;
  struct Nodo * ant;
};

struct Nodo * primero;
struct Nodo * ultimo;

struct Nodo * creaNodo(char * x){
  struct Nodo * nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
  strcpy(nuevoNodo->comando, x);
  nuevoNodo->posicion = ++cantidad;
  nuevoNodo->ant = NULL;
  nuevoNodo->sig = NULL;
  return nuevoNodo;
}
void agregar(char * x){
  struct Nodo * nuevoNodo = creaNodo(x);
  if(primero == NULL){
    primero = nuevoNodo;
    ultimo = nuevoNodo;
    return;
  }else{
    ultimo->sig = nuevoNodo;
    nuevoNodo->ant = ultimo;
    ultimo = nuevoNodo;
  }
}
void mostrar(){
  struct Nodo * temp = ultimo;
  int can;
  for(can=1;temp!= NULL&&can<11;can++){
    printf("[%d] :%s\n",temp->posicion,temp->comando);
    temp=temp->ant;
  }
}

int main(int argc, char** argv){

  char* buffer = malloc(sizeof(char));
  char** split_buffer = (char**)malloc(5*sizeof(char*));
  int size, i, flag;

  while(run==1){
    flag=1;
    printf("$prompt>>> ");
    if(fgets(buffer, MAX_LEN, stdin)!=NULL){ //Lee una linea de consola
      buffer[strcspn(buffer, "\n")] = '\0'; //remplaza el salto de linea de la cadena
      if(validate_cmd(buffer)){
        size = split_cmd(buffer, split_buffer);
        for(i=0;buffer[i] && flag==1;i++){
          printf("Leyendo: %c\n", buffer[i]);
          if(!isdigit(buffer[i]) && buffer[i]!='!')flag=0;
        }
        if(flag){
          printf("Es lo que buscabamos Houston\n");
          extraer(buffer);
          continue;
        }
        excute_cmd(split_buffer);
      }
    }

  }


  return 0;
}

int validate_cmd(char* line){//Verifica que la linea no este vacia
  return strlen(line)>=1?1:0;
}


int split_cmd(char* line, char** split_buffer){
  int i=1;
  char* n = split_buffer[0] = strtok(line, " ");//mete el comando
  while(n!=NULL){
    split_buffer[i++] = n = strtok(NULL, " ");
  }
  return i;
}

void excute_cmd(char** line){

  pid_t pid;
  agregar(line[0]);
  pid = fork();
  if(pid == 0){
    if(strcmp(line[0],"history")==0){
      mostrar();
    }
    execvp(line[0],line);
    exit(0);
  }else{
    wait(NULL);
  }
}

void extraer(char* line){
  int num = atoi(strtok(line, "!"));
  printf("Entramos buffer: %s I: %d\n", line, num);
  if(num<=cantidad){
    /*struct Nodo * temp;
    for(temp=ultimo;temp!= NULL;temp=temp->ant){
      if(temp->posicion==num){
        strcpy(line[0],temp->comando);
      }
    }*/
  }
}
