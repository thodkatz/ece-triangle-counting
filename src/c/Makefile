CC=g++ # my machine version: 10.2.0. In hpc, 7.5.0 to be compatible with cilk
CFLAGS=-g -O3 -Wall
#CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang++
CILKCC=g++-7 # my machine (AUR package)

BASE_SRC=main.c diff_time.c mm2coo.c mmio.c coo2csc.c v1.c v2.c v3.c v3_pre_cilk.c v4.c v4_yav.c 
MP_SRC=$(BASE_SRC) v3_openmp.c v3_openmp_yav.c v4_openmp.c
CILK_SRC= $(BASE_SRC) v3_cilk.c v4_cilk.c
PTHREAD_SRC=$(BASE_SRC) v4_pthread.c


all: main
	
main: $(BASE_SRC)
	$(CC) $(CFLAGS) $(BASE_SRC) -o $@ 

cilk:
	$(CILKCC) -g -O3 $(CILK_SRC) -o main_cilk -fcilkplus #-fsanitize=cilk  

cilk_hpc:
	$(CC) -g -O3 $(CILK_SRC) -o main_cilk -fcilkplus #-fsanitize=cilk  

openmp: 
	$(CC) -g -O3 $(MP_SRC) -o main_openmp -fopenmp

pthread: 
	$(CC) -g -O3 $(PTHREAD_SRC) -o main_pthread -lpthread

.PHONY: clean

clean:
	rm -f *.o main_cilk main_openmp main_pthread main
