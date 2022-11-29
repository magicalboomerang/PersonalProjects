compiler = gcc
compiler_args = -g -Wall -Wextra

driver: heap.o
	$(compiler) $(compiler_args) driver.c heap.o -o driver.out

valdriver: driver
	valgrind --leak-check=yes ./driver.out

heap.o: heap.c heap.h
	$(compiler) $(compiler_args) heap.c -c -o heap.o

clean:
	rm -f *.o *.out
