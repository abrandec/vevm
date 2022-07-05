# Vision EVM
# Author: Abran DeCarlo
# LICENSE: AGPL-3.0-ONLY

# Compiler Options
CC := clang
LIBS := -lm
FLAGS := -Oz

#####################
# 		FLAGS 		#
#####################

# Assemble CPU
CPU = src/processor/processor.c src/stack/stack.c src/bigint/bigint.c src/errors/errors.c

# EVM FLAGS
CHAIN := mainnet
FORK := arrowglacier

# DEBUG FLAG
DEBUG_MODE := 1

#####################
# 		MISC 		#
#####################

# Output dir for tests
TEST_BIN = bin/tests

#####################################################
#				Conditional Compilation				#
#####################################################

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
	CPU += src/debug/debug.c
endif

#########################################################
#						Compiling						#
#########################################################

all: vetk

tests: stack_t bigint_t processor_t	

###################################
#		  Sample Programs		  #
###################################

# VETK (Vision ETK)
vetk: sample_programs/vision_etk/main.c
	$(CC) $(EVM_FLAGS) -o bin/vetk/vetk $(CPU) sample_programs/vision_etk/main.c $(FLAGS) $(LIBS)

#############################
# 			Tests 			#
#############################

bigint_t: tests/bigint_t/bigint_t.c
	$(CC) -o $(TEST_BIN)/bigint_t tests/binint_t/binint_t.c $(FLAGS) $(LIBS)

stack_t: tests/stack_t/stack_t.c
	$(CC) -o $(TEST_BIN)/stack_t tests/stack_t/stack_t.c $(FLAGS) $(LIBS)

processor_test: tests/processor_t/processor_t.c
	$(CC) -o $(TEST_BIN)/processor_t $(EVM) tests/processor_t/processor_test.c $(FLAGS) $(LIBS)