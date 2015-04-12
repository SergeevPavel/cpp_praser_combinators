CC=g++
CFLAGS=-std=c++11 -Wall -Werror -g -I ../

all: examples

examples: examples.o
	$(CC) examples.o -o examples

examples.o: examples.cpp ../parser.h ../combinators.h ../concrete_combinators.h
	$(CC) $(CFLAGS) -c examples.cpp -o examples.o

.PHONY: clean

clean:
	rm -f examples *.o
