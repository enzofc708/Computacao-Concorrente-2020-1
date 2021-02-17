import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool; 
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.TimeUnit;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

//classe principal
public class QuickSort extends RecursiveTask<Integer> {

	private static final long serialVersionUID = 1L; //serial basico
	int inicio, fim;
	int[] arr; 

	private int divisao(int inicio, int fim, int[] arr) { 

	    int i_ = inicio, f_ = fim; 
		int pivo = new Random().nextInt(fim-inicio) + inicio; //escolhendo um pivô aleatorio

        //troca o pivô e o último elemento de lugar
		int temp = arr[f_];
		arr[f_] = arr[pivo];
		arr[pivo] = temp;
		f_--; 

		// inicio da divisao
		while (i_ <= f_) { 
			if (arr[i_] <= arr[fim]) { 
				i_++; 
				continue; 
			} 
			if (arr[f_] >= arr[fim]) { 
				f_--; 
				continue; 
			} 

			temp = arr[f_]; 
			arr[f_] = arr[i_]; 
			arr[i_] = temp; 
			f_--; 
			i_++; 
		} 

        //reloca o pivô pra posição correta
		temp = arr[f_ + 1]; 
		arr[f_ + 1] = arr[fim]; 
		arr[fim] = temp; 
		return f_ + 1; //retorna o indice do pivô
	}

	//construtor
	public QuickSort(int inicio, int fim, int[] arr) 
	{ 
		this.arr = arr; 
		this.inicio = inicio; 
		this.fim = fim; 
	} 

	//função que implementa o algoritmo em si
	@Override
	protected Integer compute() 
	{ 
		// caso base 
		if (inicio >= fim) 
			return null; 

		// divide a array
		int p = divisao(inicio, fim, arr); 

		// faz o quicksort nas duas arrays criadas 
		QuickSort esq = new QuickSort(inicio, p - 1, arr); 

		QuickSort dir = new QuickSort(p + 1, fim, arr); 

		//calcula a thread da esquerda em uma nova thread 
		esq.fork(); 
		dir.compute();

		// espera a thread da esquerda terminar 
		esq.join(); 

		return null; 
	}

	// Função principal
	public static void main(String args[]) 
	{ 
		Scanner s = new Scanner(System.in);
		int tam = 0, thr = 0;
		int[] arr = {};
		System.out.println("\n1 - Ler Array de arquivo");
		System.out.println("2 - Entrar Array manualmente");
		System.out.println("3 - Array aleatoria");
		int escolha = s.nextInt();
		switch (escolha) {
			case 1:
				System.out.println("Digite o nome do arquivo:");
				String nomeArq = s.next();
				try {
					File arq = new File(nomeArq); //abrindo arquivo
					Scanner leitor = new Scanner(arq);
					String linha = leitor.nextLine(); //lendo linha do arquivo
					String[] vetor = linha.split(" ");
					tam = vetor.length;
					arr = new int[tam];
					for(int i = 0; i < tam; i++){
						arr[i] = Integer.parseInt(vetor[i]); //convertendo pra int e adicionando na array
					}
					leitor.close();
				} catch (FileNotFoundException e) {
					System.out.println("Arquivo inexistente.");
					System.exit(1);
				}
				break;
			case 2:
				System.out.println("Digite os elementos separados por espaco:");
				String linha = s.next();
				String[] vetor = linha.split(" ");
				tam = vetor.length;
				arr = new int[tam];
				for(int i = 0; i < tam; i++){
					arr[i] = Integer.parseInt(vetor[i]);
				}
				s.nextLine();
				break;

			case 3:
				System.out.println("Digite o tamanho da array:");
				tam = s.nextInt();
			
				arr = new int[tam];
		
				//preenchendo a array
				for(int i = 0; i < tam; i++){
					arr[i] = new Random().nextInt(tam-1);
				}
				break;
		
			default:
				System.out.println("Escolha invalida");
				System.exit(1);
				break;
		}

		System.out.println("Digite o numero de Threads:");
		thr = s.nextInt();
		s.close();

		// Criando a ThreadPool
		ForkJoinPool pool = new ForkJoinPool(thr);

		//inicio do algoritmo
		long inicio = System.nanoTime();
		pool.invoke(new QuickSort(0, tam - 1, arr)); 
		long fim = System.nanoTime();

		long duracao = fim-inicio;

		//escreve o vetor em arquivo
		try{
		BufferedWriter w = new BufferedWriter(new FileWriter("output.txt"));
		w.write("");
		for(int elem : arr){
			w.write(elem + " ");
		}
		w.close();
		} catch (IOException e){
			System.exit(1);
		}


		System.out.println("Sua array foi salva em output.txt!");

		//printar tempo decorrido
		System.out.println("Tempo Decorrido(segundos): " + TimeUnit.NANOSECONDS.toMillis(duracao)/1000.0);
	} 
} 
