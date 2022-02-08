
all: malloc.so

malloc.so: malloc_trace.c
	gcc $< -o $@ -shared -ldl -fPIC

obj/%: examples/%.c
	gcc $< -o $@ -lpthread -g

mt_seq: malloc.so
	LD_PRELOAD=./malloc.so seq 1 5

mt/%: obj/% malloc.so
	LD_PRELOAD=./malloc.so $<

bb/%: %.bc bb_count.so 
	opt-11 -load ./bb_count.so -count <  $< > /dev/null

bb_count.so: bb_count.cpp
	g++ $(shell llvm-config-11 --cxxflags) -std=c++17 bb_count.cpp -c -fPIC -o  bb_count.o 
	g++ bb_count.o $(shell llvm-config-11 --ldflags --libs) -std=c++17 -shared -fPIC -o bb_count.so 

%.bc: examples/%.c
	clang-11 -c -emit-llvm $< -o $@



.PHONY: clean
clean:
	rm -f *.so *.bc obj/* a.out
