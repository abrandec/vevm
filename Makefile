
all: h_stack

tests: h_stack_test uint256_test

# stack stored in heap
h_stack: src/main.c 
	gcc -o bin/main src/main.c

h_stack_test: tests/h_stack_test.c
	gcc -o tests/bin/h_stack_test tests/h_stack_test.c

uint256_test: tests/uint256_test.c
	gcc -o tests/bin/uint256_test tests/uint256_test.c


