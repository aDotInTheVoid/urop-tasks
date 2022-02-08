#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>

typedef void *(*malloc_ptr)(size_t);
typedef void (*free_ptr)(size_t *);

malloc_ptr libc_malloc = NULL;

void *malloc(size_t size)
{
  static size_t count = 0;
  count += size;

  fprintf(stderr, "Allocation of %zu, total = %zu\n", size, count);

  if (libc_malloc == NULL)
  {
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
  }
  return (*libc_malloc)(size);
}
