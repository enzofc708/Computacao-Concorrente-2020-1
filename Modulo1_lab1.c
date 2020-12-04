#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>

#define NUMVETOR 50 //numero de elementos no vetor

typedef struct { //Struct que controla os argumentos enviado para a thread
   int inicio, fim; //inicio e fim da leitura do vetor
   int *vetor; //referencia para o vetor
} t_Args;


void *SomaVetor(void* arg){ //funcao executada na thread
  t_Args *args = (t_Args *) arg;
  for(int i = args->inicio; i <= args->fim; i++){
    args->vetor[i]++;
  }

  pthread_exit(NULL);
}

int main(){
  int meuvetor[NUMVETOR];
  int nthreads = floor(NUMVETOR/2);

  for(int i =0; i< NUMVETOR; i++){
    meuvetor[i] = i; //preenche o vetor com valores de 0 a 49
  }

   t_Args arg1, arg2;

   arg1.inicio = 0;
   arg1.fim = nthreads;
   arg1.vetor = meuvetor;
   
   arg2.inicio = nthreads + 1;
   arg2.fim = NUMVETOR - 1;
   arg2.vetor = meuvetor;

  pthread_t tid[2];
  if(pthread_create(&tid[0], NULL, SomaVetor, (void *)&arg1)){
    printf("Erro ao criar a thread");
  }
  if(pthread_create(&tid[1], NULL, SomaVetor, (void *)&arg2)){
    printf("Erro ao criar a thread");
  }

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  for(int i = 0; i < NUMVETOR; i++){
    printf("%d ",meuvetor[i]);
  }

  return 0;
}
