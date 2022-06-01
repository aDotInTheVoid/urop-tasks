#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <malloc.h>
#include <memory.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void *(*libc_malloc)(size_t size) = NULL;

void log_malloc(size_t size) {
  static atomic_size_t bytes_count = 0;
  static atomic_size_t calls_count = 0;
  size_t old_bytes = atomic_fetch_add(&bytes_count, size);
  size_t old_calls = atomic_fetch_add(&calls_count, 1);
  fprintf(stderr, "Allocation of %zu, total bytes = %zu, total calls = %zu\n",
          size, old_bytes + size, old_calls + 1);
}

void *malloc(size_t size) {
  if (libc_malloc == NULL)
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
  void *ptr = (*libc_malloc)(size);
  if (ptr != NULL)
    log_malloc(size);
  else
    fprintf(stderr, "Allocation of %zu failed\n", size);
  return ptr;
}

// Libc may call our malloc for these functions, or may not, so we must
// implement them in terms of out malloc to be accurate at counting

// From glibc docs:
// https://www.gnu.org/software/libc/manual/html_node/Allocating-Cleared-Space.html
void *calloc(size_t count, size_t eltsize) {
  void *value = reallocarray(NULL, count, eltsize);
  if (value != 0)
    memset(value, 0, count * eltsize);
  return value;
}

void *realloc(void *old_ptr, size_t new_size) {
  if (old_ptr == NULL)
    return malloc(new_size);
  if (new_size == 0) {
    free(old_ptr);
    return NULL;
  }
  size_t old_size = malloc_usable_size(old_ptr);
  void *new_ptr = malloc(new_size);
  size_t copy_size = old_size < new_size ? old_size : new_size;
  memcpy(new_ptr, old_ptr, copy_size);
  return new_ptr;
}

// From musl
// https://github.com/bminor/musl/blob/master/src/malloc/reallocarray.c
void *reallocarray(void *ptr, size_t m, size_t n) {
  if (n && m > -1 / n) {
    fprintf(stderr, "Allocation of %zu * %zu would overflow\n", m, n);
    errno = ENOMEM;
    return 0;
  }

  return realloc(ptr, m * n);
}
