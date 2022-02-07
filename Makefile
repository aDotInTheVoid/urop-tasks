
all: malloc.so

malloc.so: malloc_trace.c
	gcc $< -o $@ -shared -fPIC

mt_seq: malloc.so
	LD_PRELOAD=./malloc.so seq 1 10
