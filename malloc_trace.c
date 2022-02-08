#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>

typedef void *(*malloc_ptr)(size_t);
typedef void (*free_ptr)(size_t *);

#define BUFFER_SIZE 1000000

char buffer[BUFFER_SIZE];
int buffer_offset = 0;

size_t adjust_size(size_t size) { return size + 16 - (size % 16); }

malloc_ptr libc_malloc = NULL;
free_ptr libc_free = NULL;
bool started = false;

int id = 0;

void *malloc(size_t size)
{
  int mid = id++;
  fprintf(stderr, "%d: malloc(%zu)\n", mid, size);

  if (libc_malloc != NULL)
  {
    void *ptr = (*libc_malloc)(size);
    fprintf(stderr, "%d: Got %p with libc\n", mid, ptr);
    return ptr;
  }

  // Ensure we only call dlsym once to avoid recursion
  if (!started)
  {
    started = true;
    fprintf(stderr, "%d: Initint libc\n", mid);
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
    libc_free = dlsym(RTLD_NEXT, "free");
    void *ptr = (*libc_malloc)(size);
    fprintf(stderr, "%d: got %p (via libc init)\n", mid, ptr);
    return ptr;
  }

  // Buffer allocation path
  void *ptr = &buffer;
  size = adjust_size(size);
  ptr += buffer_offset;
  buffer_offset += size;

  fprintf(stderr, "%d: got %p (buffer)\n", mid, ptr);

  return ptr;
  //      malloc_ptr libc_ptr = (malloc_ptr)dlsym(RTLD_NEXT, "malloc");
  //     return (*libc_ptr)(size);
}

void free(void *ptr)
{
  void *b = buffer;
  if (ptr >= b && ptr <= b + BUFFER_SIZE)
  {
    return;
  }
  if (libc_free)
    (*libc_free)(ptr);
}
