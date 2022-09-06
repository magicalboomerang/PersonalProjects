source = linktester.c lint.c

build: $(objects)
	gcc -Wall -Wextra -g $(source) -o linktester

run: linktester
	./linktester

valgrind: build
	valgrind --leak-check=yes ./linktester
