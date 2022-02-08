
all: malloc.so

malloc.so: malloc_trace.c
	gcc $< -o $@ -shared -ldl -fPIC

obj/%: examples/%.c
	gcc $< -o $@

mt_seq: malloc.so
	LD_PRELOAD=./malloc.so seq 1 10

mt/%: obj/% malloc.so
	LD_PRELOAD=./malloc.so $<



.PHONY: clean
clean:
	rm -f malloc.so obj/* a.out
