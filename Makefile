CC = gcc -std=c99
CFLAGS = -Wall -g

build: tema3

tema3: graphs.c main.c
	$(CC) $(CFLAGS) $^ -o $@

run:
	./tema3 
	
clean:
	rm -f *.o *~ graf
