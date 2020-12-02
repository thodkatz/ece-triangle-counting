CC=g++
CFLAGS=-g -O3 -Wall
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang++
CILKCC2=g++-7 

BASE_SRC=main.c diff_time.c mm2coo.c mmio.c coo2csc.c v1.c v2.c v3.c v3_pre_cilk.c v4.c merge_csc.c 
MP_SRC=$(BASE_SRC) v3_openmp.c v3_openmp_playground.c
CILK_SRC= $(BASE_SRC) v3_cilk.c v4_cilk.c
PTHREAD_SRC=$(BASE_SRC) v3_pthreads.c


all: main
	
main: $(BASE_SRC)
	$(CC) $(CFLAGS) $(BASE_SRC) -o $@ 

cilk:
	$(CILKCC2) -g $(CILK_SRC) -o main -fcilkplus #-fsanitize=cilk  

openmp: 
	$(CC) -O3 $(MP_SRC) -o main -fopenmp

pthreads: 
	$(CC) -O3 $(PTHREAD_SRC) -o main -lpthread

.PHONY: clean

clean:
	rm *.o main
