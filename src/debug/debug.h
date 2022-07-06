#ifndef DEBUG_H
#define DEBUG_H

#include "../common/math/bigint/bigint.h"
#include "../core/stack/stack.h"


/*
  ┌───────────────────────────────┐
  │   MACROS                      │
  └───────────────────────────────┘
 */

#define PRINT_LENGTH 9

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   FUNCTIONS                                                                │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

// print a given buffer
// @param buffer: the buffer to print
// @param buff_name[]: name of buffer to print
// @param length: index to stop printing at
void print_buffer(uint256_t buffer[], const char buff_name[], int length);

// print debug information
// @param stack: stack to print
// @param buffer: memory to print
// @param pc: program counter
// @param gas: gas remaining
// @param opcode: opcode
void print_debug(List *stack, uint256_t memory[], int *pc, uint64_t *gas,
                 uint64_t *opcode);

/* Stack debugging */

// printfs the entire stack
// @param stack: the stack to print
void stack_print(List *stack);

// printf a specific stack index
// @param stack: which to stack to point to
// @param index: index to print from stack
void stack_peak_print(List *stack, int index);

#endif