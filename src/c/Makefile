CC=g++
CFLAGS=-O3 -g

OBJ=main.o v3.o v2.o v1.o diff_time.o

all: main
	
main: $(OBJ) 
	$(CC) $(CFLAGS) $^ -o $@ 

$(OBJ): main.h 

.PHONY: clean

clean:
	rm -r *.o main
