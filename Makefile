# Vision EVM
# Author: Abran DeCarlo
# LICENSE: AGPL-3.0-ONLY

CC = clang
FLAGS = -Oz
LIBS = -lm

EVM = src/processor.c src/stack.c src/bigint.c src/debug.c

all: vetk

tests: stack_test bigint_test processor_test	

#############################
#		  Compiling			#
#############################

# vision evm + etk (eas compiler)
vetk: sample_programs/vision_etk/main.c
	$(CC) -o bin/vetk/vetk $(EVM) sample_programs/vision_etk/main.c $(FLAGS) $(LIBS)

#############################
# 			Tests 			#
#############################

# stack stored in heap
stack_test: tests/h_stack_test.c
	$(CC) -o tests/bin/h_stack_test src/bigint.c tests/h_stack_test.c $(FLAGS) $(LIBS)

bigint_test: tests/uint256_test.c
	$(CC) -o tests/bin/uint256_test tests/uint256_test.c $(FLAGS) $(LIBS)

processor_test: tests/processor_test.c
	$(CC) -o tests/bin/processor_test $(EVM) tests/processor_test.c $(FLAGS) $(LIBS)