
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
	opt-11 -load ./bb_count.so --bb_count <  $< > /dev/null


%.so: %.cpp
	g++ $(shell llvm-config-11 --cxxflags) -std=c++17 $< -fPIC -c -o mod.o
	g++ mod.o $(shell llvm-config-11 --ldflags --libs) -std=c++17 -shared -fPIC -o $@

%.bc: examples/%.c
	clang-11 -c -emit-llvm $< -o $@

%.pdf: %.bc i_count.so graph.plot
	opt-11 -load ./i_count.so --i_count < $< > /dev/null 2> hist.dat
	gnuplot graph.plot
	mv __tmp.pdf $@



.PHONY: clean
clean:
	rm -f *.so *.bc obj/* a.out *.pdf *.dat
