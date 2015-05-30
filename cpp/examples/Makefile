CC=clang++
CFLAGS=-std=c++11 -Wall -Werror -g -I ../

all: examples expression json_parser

expression: expression.o
	$(CC) expression.o -o expression

expression.o: expression.cpp ../parser.h ../combinators.h ../concrete_combinators.h
	$(CC) $(CFLAGS) -c expression.cpp -o expression.o

json_parser: json_parser.o
	$(CC) json_parser.o -o json_parser

json_parser.o: json_parser.cpp ../parser.h ../combinators.h ../concrete_combinators.h
	$(CC) $(CFLAGS) -c json_parser.cpp -o json_parser.o

examples: examples.o
	$(CC) examples.o -o examples

examples.o: examples.cpp ../parser.h ../combinators.h ../concrete_combinators.h
	$(CC) $(CFLAGS) -c examples.cpp -o examples.o

.PHONY: clean

clean:
	rm -f examples expression json_parser *.o
