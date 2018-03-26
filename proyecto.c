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
void split_cmd(char*, char**);
void extraer(char**);
int run = 1;
int cantidad = 0;

//Metodos de lista
struct Nodo{//Crea el nodo
  char comando[30];
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
  while(run==1){

    printf("$prompt>>> ");
    if(fgets(buffer, MAX_LEN, stdin)!=NULL){ //Lee una linea de consola
      buffer[strcspn(buffer, "\n")] = '\0'; //remplaza el salto de linea de la cadena
      if(validate_cmd(buffer)){
        split_cmd(buffer, split_buffer);
        extraer(split_buffer);
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


void split_cmd(char* line, char** split_buffer){
  int i=1;
  char* n = split_buffer[0] = strtok(line, " ");//mete el comando
  while(n!=NULL){
    split_buffer[i++] = n = strtok(NULL, " ");
  }
}

void excute_cmd(char** line){

  pid_t pid;
  agregar(line[0]);
  pid = fork();
  //printf("%s\n", line[0]);
  if(pid == 0){
    if(strcmp(line[0],"history")==0){
      mostrar();
    }
    if(strcmp(line[0],"exit")==0){
      strcpy(line[0], '^C');
      printf("Adios!\n");
    }
    execvp(line[0],line);
  }else{
    wait(NULL);
    printf("Exitoso\n");
  }
}

void extraer(char** line){
  printf("%d\n",line);
  int num = atoi(&line[1]);
  printf("%d\n",line[1]);
  printf("%d\n",num);
  if(strcmp(line[0],"!")==0){printf("llego1\n");
    if(num>0&&num<=cantidad){printf("llego2\n");
      struct Nodo * temp;
      for(temp=ultimo;temp!= NULL;temp=temp->ant){
        if(temp->posicion==num){
          strcpy(line[0],temp->comando);printf("llego3\n");
        }
      }

    }
  }
}
