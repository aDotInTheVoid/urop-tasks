#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_THREADS 1000
#define N_ALLOCS 1000
#define ALLOC_SIZE 1

void *run_work(void *_) {
  void *ptrs[N_ALLOCS];
  for (int i = 0; i < N_ALLOCS; i++) ptrs[i] = malloc(ALLOC_SIZE);
  for (int i = 0; i < N_ALLOCS; i++) free(ptrs[i]);
}

int main() {
  pthread_t threads[N_THREADS];
  for (int i = 0; i < N_THREADS; i++) {
    int res = pthread_create(&threads[i], NULL, run_work, NULL);
    if (res != 0) {
      perror("pthread_create");
      exit(1);
    }
  }
  for (int i = 0; i < N_THREADS; i++) {
    int res = pthread_join(threads[i], NULL);
    if (res != 0) {
      perror("pthread_join");
      exit(1);
    }
  }
}
