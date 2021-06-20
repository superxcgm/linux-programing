all: exe

CC ?= gcc

exe: dll.o application.o dll_utls.o
	gcc -g -o exe dll.o application.o dll_utls.o

dll.o: dll.c dll.h
	$(CC) -g -c -o $@ $<

dll_utls.o: dll_utils.c dll.h
	$(CC) -g -c -o $@ $<

application.o: application.c
	$(CC) -g -c -o $@ $<

.PHONY: clean

clean:
	-rm *.o