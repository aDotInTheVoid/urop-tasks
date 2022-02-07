 #define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
 #include <dlfcn.h>

typedef void*(*malloc_ptr)(size_t);

void* malloc(size_t size) {
	fprintf(stderr, "malloc(%zu)\n", size);
	malloc_ptr libc_ptr = (malloc_ptr)dlsym(RTLD_NEXT, "malloc");
       return (*libc_ptr)(size);	
}

