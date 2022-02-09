CC ?= gcc
CXX ?= g++ 
OPT ?= opt-11
CLANG ?= clang-11
LLVMCONFIG ?= llvm-config-11

all: malloc.so

malloc.so: malloc_trace.c
	$(CC) $< -o $@ -shared -ldl -fPIC

obj/%: examples/%.c
	$(CC) $< -o $@ -lpthread -g

mt_seq: malloc.so
	LD_PRELOAD=./malloc.so seq 1 5

mt/%: obj/% malloc.so
	LD_PRELOAD=./malloc.so $<

bb/%: %.bc bb_count.so 
	$(OPT) -load ./bb_count.so --bb_count <  $< > /dev/null


%.so: %.cpp
	$(CXX) $(shell $(LLVMCONFIG) --cxxflags) -std=c++17 $< -fPIC -c -o mod.o
	$(CXX) mod.o $(shell $(LLVMCONFIG) --ldflags) -std=c++17 -shared -fPIC -o $@

%.bc: examples/%.c
	$(CLANG) -c -emit-llvm $< -o $@

%.pdf: %.bc i_count.so graph.plot
	$(OPT) -load ./i_count.so --i_count < $< > /dev/null 2> hist.dat
	gnuplot -e "filename='$<'" graph.plot  
	mv __tmp.pdf $@

.PHONY: clean
clean:
	rm -f *.so *.bc obj/* a.out *.pdf *.dat
