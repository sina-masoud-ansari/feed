CC=gcc
CFLAGS=-Wall -g
LIBS=-lcheck

NEURO_OBJS=data.o nn.o
TEST_OBJS=tests.o data_tests.o

.c.o:
	$(CC) $(CFLAGS) -c $<

all: neuro tests

neuro: main.o $(NEURO_OBJS)
	$(CC) $(CFLAGS) -o $@ main.o $(NEURO_OBJS) $(LIBS)

tests: $(NEURO_OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(NEURO_OBJS) $(TEST_OBJS) $(LIBS)

test: tests
	./tests

clean:
	rm neuro tests *.o core
