#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


//struct passada como argumento para a função executada pela thread
typedef struct meus_args{
	int id;
} Args;

//variaveis globais

int* array;
int tamArray;
int numThreads;
int tamSubArray;

// Separa a array em 2, para depois juntar de forma ordenada
// Primeira subarray vai de 1 até m
// Segunda subarray vai de m+1 até r
void merge(int arr[], int l, int m, int r)
{
	int i, j, k;  //controladores
	int n1 = m - l + 1; //tamanho do array da esquerda
	int n2 = r - m;  // tamanho do array da direita
	int L[n1], R[n2]; // arrays temporárias

	//Copiando os dados para os arrays temporários
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	//Juntando novamente e ordenando as arrays da esquerda e direita
	i = 0; // indice da array L
	j = 0; // indice da array R
	k = l; // indice da array do resultado
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

    //Copia o restante da array L
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	//Copia o restante da array R
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

//função responsável por organizar o Merge Sort
void mergeSort(int arr[], int l, int r) // "l" é o indice da array da esquerda e "r" o da direita
{
	if (l < r) {
		int m = l + (r - l) / 2; // escrevemos dessa forma para evitar overflow se "l" e "h" forem grandes

		// Sort nas subarrays da esquerda e direita
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);

        //Junta as duas subarrays
		merge(arr, l, m, r);
	}
}

//função que controla a divisão da tarefa para as threads
void *mergeSortThread(void *args){
	Args* meu_arg = (Args *) args;
	int id = meu_arg->id;
	int inicio = id * tamSubArray;
	int fim = (id + 1) * tamSubArray - 1;
	int meio = inicio + (fim - inicio) / 2;
	if(fim >= tamArray - 1){ //controla o resto da matriz caso o numero de threads não seja
		mergeSort(array, inicio, meio);//múltiplo do tamanho da array
		mergeSort(array, meio+1, tamArray - 1);
		merge(array, inicio, meio, tamArray - 1);
	}
	else{
		mergeSort(array, inicio, meio);//múltiplo do tamanho da array
		mergeSort(array, meio+1, fim);
		merge(array, inicio, meio, fim);
	}
	pthread_exit(NULL);
} 

// função simples para printar o conteúdo da array para testes
void printArray(int A[], int size){
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", A[i]);
	printf("\n");
}



int main(int argc, char** argv){

	double start, finish, elapsed; //contagem do tempo

	if(argc != 3){
		printf("Uso: ./%s <NUM_THREADS> <TAM_ARRAY>\n", argv[0]);
		exit(-1);
	}

	numThreads = atoi(argv[1]);
	tamArray = atoi(argv[2]);
	pthread_t tid[numThreads];

	array = (int *) malloc(sizeof(int) * tamArray); //alocação de memória para a matriz
	if(array == NULL) {puts("Erro no  malloc"); exit(-1);}

	for(int i = 0; i < tamArray; i++){ //preenchendo a matriz
		array[tamArray - i - 1] = i;
	}
	//puts("Array antes da ordenacao");
	//printArray(array, tamArray);

	tamSubArray = tamArray / numThreads;

	GET_TIME(start);

	//inicializando as threads
	for(int i = 0; i < numThreads; i++){
		Args *meu_arg = (Args *) malloc(sizeof(Args));
		meu_arg->id = i;
		if(pthread_create(tid + i, NULL, mergeSortThread, (void *) meu_arg))
      		puts("Erro no pthread");
	}

	for(int i = 0; i < numThreads; i++){
		pthread_join(*(tid + i), NULL);
	}

	//fazendo o merge das partes finais
	merge(array, 0, (tamArray / 2 - 1) / 2, tamArray / 2 - 1); 
    merge(array, tamArray / 2, tamArray/2 + (tamArray-1-tamArray/2)/2, tamArray - 1); 
    merge(array, 0, (tamArray - 1)/2, tamArray - 1);
	
	GET_TIME(finish);
	elapsed = finish - start;

	//puts("Array apos a ordenacao");
	//printArray(array, tamArray);
	printf("tempo: %lf", elapsed);
}
