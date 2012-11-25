CC=gcc
CFLAGS=-Wall -g

all: main.c data.c
	clear
	$(CC) $(CFLAGS) main.c data.c nn.c -o neuro

test: tests.c
	clear
	$(CC) $(CFLAGS) tests.c -o tests -lcheck
	./tests 

clean:
	rm neuro test *.o


