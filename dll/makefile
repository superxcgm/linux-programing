all: exe

CC ?= gcc

CXXFLAG = -g -c -I./include

exe: application.o libdll.a
	gcc -g -o exe application.o -L . -ldll

libdll.a: dll.o dll_utils.o
	ar rs libdll.a dll.o dll_utils.o

dll.o: src/dll.c include/dll.h
	$(CC) $(CXXFLAG) -o $@ $<

dll_utils.o: src/dll_utils.c include/dll.h
	$(CC) $(CXXFLAG) -o $@ $<

application.o: src/application.c
	$(CC) $(CXXFLAG) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o *.so *.a exe exe_so
