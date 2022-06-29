# Vision EVM
# Author: Abran DeCarlo
# LICENSE: AGPL-3.0-ONLY

#
FLAGS := -std=c11 -O2

# compiling
all: main

tests: h_stack_test uint256_test processor_test	

main: src/main.c
	gcc -o bin/main src/main.c $(FLAGS)

# stack stored in heap
h_stack_test: tests/h_stack_test.c
	gcc -o tests/bin/h_stack_test tests/h_stack_test.c

uint256_test: tests/uint256_test.c
	gcc -o tests/bin/uint256_test tests/uint256_test.c

processor_test: tests/processor_test.c
	gcc -o tests/bin/processor_test tests/processor_test.c
