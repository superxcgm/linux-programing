#!/bin/sh

set -e

gcc -c application.c -o application.o
gcc -c -fPIC dll.c -o dll.o
gcc -c -fPIC dll_utils.c -o dll_utils.o

gcc dll.o dll_utils.o -shared -o libdll.so

sudo mv libdll.so /usr/local/lib

gcc -o exe_so application.o -ldll