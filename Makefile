CC = gcc
CFLAGS = -Wall -g

all: coordinator agent main

coordinator: coordinator.c
	$(CC) $(CFLAGS) -c coordinator.c

agent: agent.c
	$(CC) $(CFLAGS) -c agent.c

main: main.c coordinator.o agent.o
	$(CC) $(CFLAGS) main.c coordinator.o agent.o -o koordynator

clean:
	rm -f *.o koordynator
