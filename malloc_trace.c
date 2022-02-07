#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>

typedef void *(*malloc_ptr)(size_t);

#define BUFFER_SIZE 1000000

char buffer[BUFFER_SIZE];
int buffer_offset = 0;

size_t adjust_size(size_t size) { return size + 16 - (size % 16); }

void *malloc(size_t size) {
  fprintf(stderr, "malloc(%zu)", size);

  void *ptr = &buffer;
  size = adjust_size(size);
  ptr += buffer_offset;
  buffer_offset += size;

  fprintf(stderr, " = %p\n", ptr);
  fflush(stderr);
  return ptr;
  //      malloc_ptr libc_ptr = (malloc_ptr)dlsym(RTLD_NEXT, "malloc");
  //     return (*libc_ptr)(size);
}

void free(void *ptr) {
  // lmao
}
