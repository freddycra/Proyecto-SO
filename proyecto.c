#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LEN 100
#define CMD_LEN 4

int validate_cmd(char*); //correct[1] incorrect[0]
int excute_cmd(char**);
int split_cmd(char*, char**);

//Metodos de historial
typedef struct Nodo{//Crea el nodo
  char* comando;
  struct Nodo * sig;
}Nodo;

typedef struct{
  Nodo * primero;
  Nodo * ultimo;
  int cantidadComandos;
}Lista;

void lista_init(Lista* list){
  list->primero = NULL;
  list->ultimo = NULL;
  list->cantidadComandos = 0;
}


void eliminar_final(Lista* list){
  int i = 1;
  Nodo* aux = list->primero;
  while(i<CMD_LEN){
    aux=aux->sig;
    i++;
  }
  list->ultimo = aux;
  aux=aux->sig;
  free(aux->comando);
  free(aux);
  list->ultimo->sig = NULL;
}

void agregar(char* cmd, Lista* list){
  Nodo* nodo;
  nodo = (Nodo*)malloc(sizeof(Nodo));
  nodo->comando = (char*)malloc(MAX_LEN * sizeof(char));
    if(list->primero==NULL){
      strcpy(nodo->comando, cmd);
      list->primero = list->ultimo = nodo;
    }else{
      if(list->cantidadComandos>CMD_LEN)
        eliminar_final(list);
      strcpy(nodo->comando, cmd);
      nodo->sig = list->primero;
      list->primero = nodo;
    }
    list->cantidadComandos++;
}


//imprime la lista
void history(Lista* list){
  int i=0, num = list->cantidadComandos;
  Nodo* aux = list->primero;
  while(i < 10 && aux != NULL){
    printf("%d\t%s\n",num--,aux->comando);
    aux=aux->sig;
    i++;
  }
}

int main(int argc, char** argv){

  int run=1, size;
  Lista* list = malloc(sizeof(Lista));
  lista_init(list);
  char* buffer = malloc(sizeof(char));
  char** split_buffer = (char**)malloc(5*sizeof(char*));
  while(run){

    printf("$prompt>> ");
    if(fgets(buffer, MAX_LEN, stdin)!=NULL){ //Lee una linea de consola
      buffer[strcspn(buffer, "\n")] = '\0'; //remplaza el salto de linea de la cadena
      if(validate_cmd(buffer)){
        agregar(buffer, list);
        size = split_cmd(buffer, split_buffer);
        if(strcmp(split_buffer[0],"history")==0){
          history(list);
          continue;
        }
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

int split_cmd(char* line, char** split_buffer){
  int i=1;
  char* n = split_buffer[0] = strtok(line, " ");//mete el comando
  while(n!=NULL){
    split_buffer[i++] = n = strtok(NULL, " ");
  }
  return i;
}

int excute_cmd(char** line){
  pid_t pid = fork();
  if(pid == 0){
    execvp(line[0],line);
    exit(0);
  }else{
    wait(NULL);
  }
}
