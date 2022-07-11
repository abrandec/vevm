#ifndef VM_H
#define VM_H

#include "../../common/math/bigint/bigint.h"
#include "../config.h"

#include <inttypes.h>

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   FUNCTIONS                                                                │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

// Entry point for EVM
// @param program[]: program to run
// @param memory[]: memory to run program in
// @param pc: program counter
// @param opcode: opcode
// @param gas: gas remaining
// @param mem_expanded: whether memory is expanded
// @param mem_end: end of memory
void _vm(uint256_t program[], uint256_t memory[], int *pc, int max_pc, uint64_t *opcode,
         uint64_t *gas, bool *mem_expanded, uint64_t *mem_end);

// wrapper for _vm
// @param program[]: program to run
void run_vm(uint256_t program[]);

// set a buffer to zero
// @param buffer: the buffer to clear
// @param start: the starting index to set to zero
// @param end: the ending index to set to zero
void clear_buffer(uint256_t buffer[], int start, int end);

#endif