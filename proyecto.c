#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#define MAX_LEN 100
#define CMD_LEN 10

typedef struct Nodo{//Crea el nodo
  char* comando;
  struct Nodo * sig;
}Nodo;

typedef struct{
  Nodo * primero;
  Nodo * ultimo;
  int cantidadComandos;
}Lista;

int validate_cmd(char*); //correct[1] incorrect[0]
int excute_cmd(char**, int);
int split_cmd(char*, char**);
void extraer(char*, Lista*);
int background(char*);

//Metodos de historial
void lista_init(Lista* list){
  list->primero = NULL;
  list->ultimo = NULL;
  list->cantidadComandos = 0;
}

void agregar(char* cmd, Lista* list){
  Nodo* nodo;
  nodo = (Nodo*)malloc(sizeof(Nodo));
  nodo->comando = (char*)malloc(MAX_LEN * sizeof(char));
    if(list->primero==NULL){
      strcpy(nodo->comando, cmd);
      list->primero = list->ultimo = nodo;
    }else{
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
  while(i <= CMD_LEN && aux != NULL){
    printf("%d\t%s\n",num--,aux->comando);
    aux=aux->sig;
    i++;
  }
}

int main(int argc, char** argv){

  int run=1, i, flag=1, size;
  Lista* list = malloc(sizeof(Lista));
  lista_init(list);
  char* buffer = malloc(sizeof(char));
  char** split_buffer = (char**)malloc(5*sizeof(char*));

  while(run){
    flag=1;
    printf("$prompt>> ");
    if(fgets(buffer, MAX_LEN, stdin)!=NULL){ //Lee una linea de consola
      buffer[strcspn(buffer, "\n")] = '\0'; //remplaza el salto de linea de la cadena
      if(validate_cmd(buffer)){
        agregar(buffer, list);
        size=background(buffer);
        split_cmd(buffer, split_buffer);
        if(strcmp(split_buffer[0],"history")==0){
          history(list);
          continue;
        }
        for(i=1;buffer[i] && flag==1;i++)
          if(!isdigit(buffer[i]))flag=0;
        if(flag){
          extraer(buffer, list);
          continue;
        }
        excute_cmd(split_buffer, size);
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

int excute_cmd(char** line, int size){
  pid_t pid = fork();
  if(pid == 0){
    execvp(line[0],line);
    exit(0);
  }else{
    if(size==0)
      wait(NULL);
  }
}

void extraer(char* line, Lista* list){
  if(line[1]!='\0'){
    int num = atoi(strtok(line, "!")), i;
    i=list->cantidadComandos-num;
    if(num<=list->cantidadComandos){
      Nodo * temp = list->primero;
      while(i>0){
        temp=temp->sig;
        i--;
      }
      printf("%s\n", temp->comando);
    }
  }
}

int background(char* line){
  int i=0;
  for(i;line[i]!='\0';i++){}
  if(line[i-1]=='&'){
    line[strcspn(line, "&")] = '\0';
    return i;
  }else
    return 0;
}
