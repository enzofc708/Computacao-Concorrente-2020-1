#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int dim;
int numthreads;
int* matriz1;
int* matriz2;
int* saida;

double start, finish, elapsed;

void *multip (void *args){
  int id = *(int *) args;
  for(int i = id; i < dim; i = i + numthreads){
    for(int j = 0; j < dim; j++){
      for(int k = 0; k < dim; k++){
        saida[i*dim +j] += matriz1[i*dim + k] * matriz2[j + k*dim];
      }
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]){
  GET_TIME(start);
  //tratamento da chamada
  if(argc != 3){
    printf("Utilizacao: %s <dimensao_da_matriz> <numero_de_threads>\n", argv[0]);
    return 2;
  }

  dim = atoi(argv[1]);
  numthreads = atoi(argv[2]);

  //alocacao de memoria
  matriz1 = (int *) malloc(sizeof(int) * dim * dim);
  if(matriz1 == NULL) {puts("Erro no  malloc"); return 2;}
  matriz2 = (int *) malloc(sizeof(int) * dim * dim);
  if(matriz2 == NULL) {puts("Erro no  malloc"); return 2;}
  saida = (int *) malloc(sizeof(int) * dim * dim);
  if(saida == NULL) {puts("Erro no  malloc"); return 2;}

  pthread_t tid[numthreads];
  int ids[numthreads];

  //inicializacao das matrizes
  for(int i = 0; i < dim; i++){
    for(int j = 0; j < dim; j++){
      matriz1[i*dim + j] = i*dim + j;
      matriz2[i*dim + j] = i*dim + j;
    }
    saida[i] = 0;
  }

  for(int i = 0; i < numthreads; i++){
    ids[i] = i;
  }


  //calculo da multiplicacao

  for(int i = 0; i < numthreads; i++){
    if(pthread_create(tid + i, NULL, multip, (void *) ids + i))
      puts("Erro no pthread");
  }

  for(int i = 0; i < numthreads; i++){
    pthread_join(*(tid + i), NULL);
  }

  GET_TIME(finish);
  elapsed = finish-start;
  printf("%.2f", elapsed);
  return 0;
}
