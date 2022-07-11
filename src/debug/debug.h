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
// @param buffer: memory to print
// @param pc: program counter
// @param gas: gas remaining
// @param opcode: opcode
void print_debug(uint256_t memory[], int *pc, uint64_t *gas,
                 uint64_t *opcode);

/* Stack debugging */



#endif