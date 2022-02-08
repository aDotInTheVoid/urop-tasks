
all: malloc.so

malloc.so: malloc_trace.c
	gcc $< -o $@ -shared -fPIC

obj/%: examples/%.c
	gcc $< -o $@

mt_seq: malloc.so
	LD_PRELOAD=./malloc.so seq 1 10

mt_alloc_allot: malloc.so obj/alloc_alot
	LD_PRELOAD=./malloc.so obj/alloc_alot

.PHONY: clean
clean:
	rm -f malloc.so obj/* a.out