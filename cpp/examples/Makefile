CC=clang++
CFLAGS=-std=c++11 -Wall -Werror -g -I ../

all: examples expression

expression: expression.o
	$(CC) expression.o -o expression

expression.o: expression.cpp ../parser.h ../combinators.h ../concrete_combinators.h
	$(CC) $(CFLAGS) -c expression.cpp -o expression.o

examples: examples.o
	$(CC) examples.o -o examples

examples.o: examples.cpp ../parser.h ../combinators.h ../concrete_combinators.h
	$(CC) $(CFLAGS) -c examples.cpp -o examples.o

.PHONY: clean

clean:
	rm -f examples expressions *.o
