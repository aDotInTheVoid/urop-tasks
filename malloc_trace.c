#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>

typedef void *(*malloc_ptr)(size_t);
typedef void (*free_ptr)(size_t *);

malloc_ptr libc_malloc = NULL;

void log_malloc(size_t size) {
  static atomic_size_t count = 0;
  size_t old_count = atomic_fetch_add(&count, size);
  fprintf(stderr, "Allocation of %zu, total = %zu\n", size, old_count + size);
}

void *malloc(size_t size) {
  log_malloc(size);
  if (libc_malloc == NULL) {
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
  }
  return (*libc_malloc)(size);
}

void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void *reallocarray(void *ptr, size_t nmemb, size_t size);
