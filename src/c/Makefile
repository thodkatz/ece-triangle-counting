CC=g++
CFLAGS=-O3 -g -Wall
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang++

OBJ=v3.o v2.o v1.o diff_time.o coo2csc.o 

MP_SRC=main.c diff_time.c mm2coo.c mmio.c v1.c v2.c v3.c coo2csc.c
CILK_SRC= main.c diff_time.c mm2coo.c mmio.c v1.c v2.c v3.c v3_pre_cilk.c v3_cilk.c coo2csc.c


all: main
	
main: $(OBJ) mmio.o mm2coo.o main.o v3_pre_cilk.o
	$(CC) $(CFLAGS) $^ -o $@ 
	rm -r *.o

$(OBJ): include/main.h 
main.o: include/main.h include/mmio.h
mm2coo.o: include/main.h include/mmio.h
mmio.o: include/mmio.h

cilk: 
	$(CC) -O3 $(CILK_SRC) -o main -fcilkplus #-fsanitize=cilk #-Og

openmp: 
	$(CC) -O3 

.PHONY: clean

clean:
	rm *.o main
