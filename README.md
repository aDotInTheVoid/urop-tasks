# Nixon's UROP Application Tasks

<https://www.doc.ic.ac.uk/~cristic/misc/urop-tasks.txt>

- gcc/g++
- Gnu make
- LLVM-11 (including headers)
- gnuplot

## [1: `malloc` Tracing](malloc_trace.c)

Building: `make malloc.so` or `gcc malloc_trace.c -o malloc.so -shared -ldl -fPIC`

Running: `LD_PRELOAD=./malloc.so seq 1 10`

To run one of the [examples](examples), so `make mt/other_allocs` where
`examples/other_allocs.c` is a stand alone c file. If you want to try on your
own `c` files, place them in the examples folder.

This has been tested on the lab machines (ubuntu 20.04) and my laptop (ubuntu 21.10), but
not on musl, so it may rely on glibc implementation details (eg `fprintf` not calling `malloc`)

[It has been tested to work in across threads](examples/alloc_in_parallel.c), [and with realloc/reallocarray/calloc](examples/other_allocs.c)

## [2a: LLVM Function and Basic Block Counter](bb_count.cpp)

This relies on having LLVM 11 installed, as that was the version the task specified.

If you happen to be on the happy path (ubuntu, llvm-11-dev and clang-11
installed), run `make bb/program`, which similarly works for any freestanding c
program in the examples folder.

You make notice a `rm program.bc` output after the relevent output ("The program has a total of 1 functions and 10 basic blocks."). This is due to how gnu make handles [intermediate files](https://www.gnu.org/software/make/manual/make.html#Chained-Rules).

If your not on the happy path, you can point to `Makefile` at llvm yourself. You will need `opt`, `clang`, and `llvm-config`.

Then run something like:
```
make CLANG=$BB/llvm-11/bin/clang LLVMCONFIG=$BB/llvm-11/bin/llvm-config OPT=$BB/llvm-11/bin/opt bb/program
```


Where `$BB/llvm-11` is the prefix where you have llvm-11 sitting.

## [2b: Histogram of instructions in a basic block](i_count.cpp)

LLVM should be installed and pointed to as above. This also requires gnuplot.

If you run `make program.pdf`, it should produce an histogram in `program.pdf`

We can use [godbolt](https://godbolt.org/z/16doa1eKc) to confirm that this is correct.

For a more representive sample, here is the output on the sqlite amalgamation (3.37.2)



If any of this doesnt work, please email me, because it was quite brittle to build.