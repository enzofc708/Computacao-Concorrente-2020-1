#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4


int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 1 */
void *A (void *t) {
  pthread_mutex_lock(&x_mutex);
  x++;
  puts("tudo bem?");
  if (x==2) {
      pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread 2 */
void *B (void *t) {
  pthread_mutex_lock(&x_mutex);
  x++;
  puts("bom dia!");
  if (x==2) {
    pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread 3 */
void *C (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x < 2) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  puts("até mais!");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread 4 */
void *D (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x < 2) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  puts("boa tarde!");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  pthread_create(&threads[2], NULL, A, NULL);
  pthread_create(&threads[3], NULL, B, NULL);
  pthread_create(&threads[0], NULL, C, NULL);
  pthread_create(&threads[1], NULL, D, NULL);

  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
