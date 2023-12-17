CC=gcc
CFLAGS=-I.

UTILS = simplecrc.c simplecrc.h

all: PT 


simplemake: simplecrc_test.c simplecrc_test.h $(UTILS)
	$(CC) -o simplecrc_test.o simplecrc_test.c simplecrc.c $(CFLAGS)

PT: ecptGen.c $(UTILS)
	$(CC) -o ecptGen.o ecptGen.c $(UTILS)

test: test.c
	$(CC) -o test.o test.c

clean:
	rm -rf *.o

