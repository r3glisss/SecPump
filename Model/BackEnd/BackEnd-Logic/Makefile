CC=gcc
CFLAGS=-m32 -Wall -pedantic -fsanitize=address
EXEC=main

all: $(EXEC)

main: main.o 
	$(CC) -o $@ $^ $(CFLAGS)

main.o : main.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf main 
