CC=g++
CFLAGS=-O3 -g


all: v3
	
v3: *.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $<

.PHONY: clean

clean:
	rm -r *.o v3
