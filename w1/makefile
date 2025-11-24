CC=gcc
CFLAGS=-Wall

all: w1

w1: w1.o
	$(CC) $(CFLAGS) -o w1 w1.o

w1.o: w1.c
	$(CC) $(CFLAGS) -c w1.c

clean:
	rm -f *.o w1
