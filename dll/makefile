all: exe

CC ?= gcc

exe: application.o libdll.a
	gcc -g -o exe application.o -L . -ldll

libdll.a: dll.o dll_utils.o
	ar rs libdll.a dll.o dll_utils.o

dll.o: dll.c dll.h
	$(CC) -g -c -o $@ $<

dll_utils.o: dll_utils.c dll.h
	$(CC) -g -c -o $@ $<

application.o: application.c
	$(CC) -g -c -o $@ $<

.PHONY: clean

clean:
	rm -f *.o *.so *.a exe exe_so