#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>

typedef void *(*malloc_ptr)(size_t);
typedef void (*free_ptr)(size_t *);

malloc_ptr libc_malloc = NULL;

void *malloc(size_t size)
{
  fprintf(stderr, "malloc(%zu)\n", size);

  if (libc_malloc == NULL)
  {
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
  }
  return (*libc_malloc)(size);
}
