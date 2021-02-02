import java.util.Random;

class MeuBuffer{ //classe que armazena o buffer
  public static int[] arr = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; //nosso buffer armazenará apenas inteiros
  private int elementos;


  MeuBuffer(){}

  public static void printar(){ //mostra os elementos do buffer
    System.out.print("Buffer: ");
    for(int i = 0; i < 10; i++){
      System.out.print(arr[i] + " ");
    }
    System.out.println("");
  }

  public void inserir(int indice, int elemento){ //insere um elemento no buffer
    if(arr[indice] != -1){
      System.out.println("Erro, posicao " + indice + " ocupada");
    }
    else{
      arr[indice] =  elemento;
      elementos++;
      System.out.println("Inserido " + elemento + " na posicao " + indice);
    }
  }

  public void retirar(int indice){ //remove um elemento do buffer
    if(arr[indice] == -1){
      System.out.println("Erro, posicao " + indice + " vazia");
    }
    else{
      elementos--;
      System.out.println("Removido " + arr[indice] + " na posicao " + indice);
      arr[indice] = -1;
    }
  }

  public int getElementos(){ //retorna o num de elementos no buffer
    return elementos; 
  }

}

// Monitor
class CP {
  private int cons, prods;  
  
  // Construtor
  CP() { 
     this.cons = 0; //consumidores
     this.prods = 0; //escritores
  } 
  
  // Entrada para consumidores
  public synchronized void EntraConsumidor (int id) {
    try { 
      while ((this.cons > 0) || (this.prods > 0)) {
         System.out.println ("Consumidor " + id + " bloqueado");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.cons++;  //registra que ha mais um leitor lendo
      System.out.println ("Consumidor "+id+" ativo");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiConsumidor (int id) {
     this.cons--; //registra que um leitor saiu
     if (this.cons == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     System.out.println ("COnsumidor "+id+" saindo");
  }
  
  // Entrada para escritores
  public synchronized void EntraProdutor (int id) {
    try { 
      while ((this.cons > 0) || (this.prods > 0)) {
         System.out.println ("Produtor "+id+" bloqueado");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.prods++; //registra que ha um escritor escrevendo
      System.out.println ("Produtor " + id + " ativo");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiProdutor (int id) {
     this.prods--; //registra que o produtor saiu
     notify(); //libera a proxima thread
     System.out.println ("Produtor "+id+" saindo");
  }
}

//--------------------------------------------------------
// Leitor
class Consumidor extends Thread {
  int id; //identificador da thread
  int elemento; //elemento a ser inserido
  int indice;
  CP monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Consumidor (int id, int indice, CP m){
    this.id = id;
    this.monitor = m;
    this.indice = indice;
  }

  // Método executado pela thread
  public void run () {
    this.monitor.EntraConsumidor(this.id);
    MeuBuffer.arr[indice] = -1;
    this.monitor.SaiConsumidor(this.id);
  }
}

//--------------------------------------------------------
// Escritor
class Produtor extends Thread {
  int id; //identificador da thread
  int elemento;
  CP monitor; //objeto monitor para coordenar a lógica de execução das threads
  int indice;

  // Construtor
  Produtor (int id, int indice, int elemento, CP m) {
    this.id = id;
    this.elemento = elemento;
    this.monitor = m;
    this.indice = indice;
  }

  // Método executado pela thread
  public void run () {
    this.monitor.EntraProdutor(this.id); 
    MeuBuffer.arr[indice] = elemento;
    this.monitor.SaiProdutor(this.id);
  }
}

//--------------------------------------------------------
// Classe principal
class ConsumidorProdutor {
  static final int C = 4;
  static final int P = 3;

  public static void main (String[] args) {
    int i;
    CP monitor = new CP();            // Monitor (objeto compartilhado entre leitores e escritores)
    Consumidor[] c = new Consumidor[C];       // Threads leitores
    Produtor[] p = new Produtor[P];   // Threads escritores

    Random gerador = new Random();

    //inicia o log de saida
    System.out.println ("Buffer Inicializado");
    
    for (i=0; i<C; i++) {
       int indice = gerador.nextInt(10);
       c[i] = new Consumidor(i+1, indice, monitor);
       c[i].start(); 
    }
    for (i=0; i<P; i++) {
       int indice = gerador.nextInt(10);
       int elemento = gerador.nextInt(40);
       p[i] = new Produtor(i+1, indice, elemento, monitor);
       p[i].start(); 
    }
    for (i=0; i<C; i++) {
      try{c[i].join();}
      catch(InterruptedException e){return;}

   }
   for (i=0; i<P; i++) {
    try{p[i].join();}
    catch(InterruptedException e){return;}
   }
    MeuBuffer.printar();
  }

}
