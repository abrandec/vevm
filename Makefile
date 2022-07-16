# Vision EVM
# Author: Abran DeCarlo
# LICENSE: AGPL-3.0-ONLY

# Compiler Options
CC := clang
LIBS := -lm
FLAGS := -Oz

# Local Libraries

UTILS := src/errors/errors.c src/common/utils/hex_utils/hex_utils.c src/common/cmd/cmd.c src/common/cmd/argparse/argparse.c src/common/io/io.c

MATH := src/common/math/bigint/bigint.c

# Assemble VM
VM := $(MATH) src/core/stack/stack.c src/core/vm/vm.c 

# Assemble Tests
TESTS := tests/common_t/math_t/bigint_t/bigint_t.c tests/core_t/stack_t/stack_t.c tests/core_t/vm_t/vm_t.c tests/test_utils/assert.c tests/common_t/io_t/io_t.c

#					
# 		FLAGS 		
#					

# EVM FLAGS
CHAIN := mainnet
FORK := arrowglacier

# OPTIONS (PLEASE DEPRECATE)
OPTIONS := 

# DEBUG FLAG
DEBUG_MODE := 1

#					
# 		MISC 		
#					

#												
#				Conditional Compilation			
#													

# Compilation flags for opcodes & gas table
ifeq ($(CHAIN), mainnet)
	EVM_FLAGS = -DMAINNET
	ifeq ($(FORK), arrowglacier)
		EVM_FLAGS += -DARROW_GLACIER
	endif
endif

# Compilation flag for debugging
ifeq ($(DEBUG_MODE), 1)
	OPTIONS += -DDEBUG
	VM += src/debug/debug.c
endif

#  											
#						Compiling					
#													

all: vevm

tests: main_t	

#								 
#		  Sample Programs		
#								 

# VEVM (Vision EVM)
vevm: src/vevm/main.c 
	$(CC) $(EVM_FLAGS) -o bin/vevm/vevm  $(UTILS) $(VM) $(OPTIONS) src/vevm/main.c $(FLAGS) $(LIBS)

#   					
# 			Tests 		
#							

# Main Test Suite
main_t: tests/main_t.c
	$(CC) $(EVM_FLAGS) -o bin/tests/main_t $(UTILS) $(VM) tests/main_t.c $(TESTS) -DTEST $(FLAGS) $(LIBS)