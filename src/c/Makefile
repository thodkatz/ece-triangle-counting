CC=g++
CFLAGS=-O3 -g -Wall

OBJ=v3.o v2.o v1.o diff_time.o coo2csc.o 

all: main
	
main: $(OBJ) mmio.o mm2coo.o main.o
	$(CC) $(CFLAGS) $^ -o $@ 
	rm -r *.o

$(OBJ): include/main.h 
main.o: include/main.h include/mmio.h
mm2coo.o: include/main.h include/mmio.h
mmio.o: include/mmio.h

.PHONY: clean

clean:
	rm main
