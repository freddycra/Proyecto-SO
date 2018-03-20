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


int numero = 10;

//Metodos de lista
struct Nodo{//Crea el nodo
  char comando[30];
  int n;
  struct Nodo * sig;
  struct Nodo * ant;
};

struct Nodo * primero;
struct Nodo * ultimo;

struct Nodo * creaNodo(char * x){
  struct Nodo * nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
  strcpy(nuevoNodo->comando, x);
  nuevoNodo->n = numero--;
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
    //printf("[%d] :%s\n",can,temp->comando);
    printf("[%d] :%s\n",temp->n, temp->comando);
    temp=temp->ant;
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
        agregar(split_buffer[0]);
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

int excute_cmd(char** line){
  pid_t pid = fork();
  if(pid == 0){
    printf("%s\n",line[0]);
    if(strcmp(line[0],"history")==0){
      mostrar();
      return;
    }
    execvp(line[0],line);
  }else{
    wait(NULL);
    printf("Exitoso\n");
  }
}
