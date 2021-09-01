#!/bin/sh

set -e
# todo: fix...
gcc -I./include -c src/application.c -o application.o
gcc -I./include -c -fPIC src/dll.c -o dll.o
gcc -I./include -c -fPIC src/dll_utils.c -o dll_utils.o

gcc dll.o dll_utils.o -shared -o libdll.so

sudo mv libdll.so /usr/local/lib

gcc -o exe_so application.o -ldll
