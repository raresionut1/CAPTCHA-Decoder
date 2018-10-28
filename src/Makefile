CC=gcc
CFLAGS=-Wall -Wextra -c
EXE=poza

build: poza.o bmp_header.h
	$(CC) $^ -o $(EXE)

poza.o: poza.c
	$(CC) $(CFLAGS) $^ -o $@

run:
	./$(EXE)

clean:
	rm -rf poza.o $(EXE)


