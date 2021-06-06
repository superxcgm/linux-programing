all: exe

CC ?= gcc

exe: dll.o application.o
	gcc -o exe dll.o application.o

dll.o: dll.c dll.h
	$(CC) -c -o $@ $<

application.o: application.c
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	-rm *.o