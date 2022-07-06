# Vision EVM
# Author: Abran DeCarlo
# LICENSE: AGPL-3.0-ONLY

# Compiler Options
CC := clang
LIBS := -lm
FLAGS := -Oz

#					#
# 		FLAGS 		#
#					#

# Assemble VM
VM := src/common/math/bigint/bigint.c src/core/stack/stack.c src/core/vm/vm.c src/errors/errors.c

# IO
IO := src/common/io/io.c

# EVM FLAGS
CHAIN := mainnet
FORK := arrowglacier

# DEBUG FLAG
DEBUG_MODE := 1

#					#
# 		MISC 		#
#					#

# Output dir for tests
TEST_BIN = bin/tests

#													#
#				Conditional Compilation				#
#													#		

# Compilation flags for opcodes & gas table
ifeq ($(CHAIN), mainnet)
	EVM_FLAGS = -DMAINNET
	ifeq ($(FORK), arrowglacier)
		EVM_FLAGS += -DARROW_GLACIER
	endif
endif

# Compilation flags for debugging
ifeq ($(DEBUG_MODE), 1)
	FLAGS += -DDEBUG
	VM += src/debug/debug.c
endif

#  														#
#						Compiling						#
#														#

all: vetk

tests: main_t	

#								  #
#		  Sample Programs		  #
#								  #

# VETK (Vision ETK)
vetk: sample_programs/vision_etk/main.c 
	$(CC) $(EVM_FLAGS) -o bin/vetk/vetk $(IO) $(VM) sample_programs/vision_etk/main.c $(FLAGS) $(LIBS)

#   						#
# 			Tests 			#
#							#		

main_t: tests/common_t/math_t/bigint_t/bigint_t.c tests/core_t/stack_t/stack_t.c tests/core_t/vm_t/vm_t.c
	$(CC) $(EVM_FLAGS) -o $(TEST_BIN)/main_t $(VM) tests/main_t.c $(FLAGS) $(LIBS)