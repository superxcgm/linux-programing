#!/usr/bin/bash

set -e

gcc mem-layout.c -o mem-layout
size mem-layout

# show stack memory max size, show in KB on ubuntu 20.04
echo 'stack memory max size(kb)'
ulimit -s

# do not allocate big array in function which may cause stack overflow

echo 'stack corruption'
gcc stack-corruption.c -o stack-corruption
./stack-corruption

rm -f mem-layout
rm -f stack-corruption