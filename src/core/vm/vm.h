#ifndef VM_H
#define VM_H

#include "../../common/math/bigint/bigint.h"
#include "../config.h"

#include <inttypes.h>

typedef struct vm_state_t {
  uint64_t pc;
  uint64_t max_pc;
  uint64_t opcode;
  uint64_t gas;
  uint64_t mem_end;
  bool mem_expanded;
} vm_state_t;

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
// @param vm: VM state
void _vm(uint256_t program[], uint256_t memory[], vm_state_t *vm_state);

// wrapper for _vm
// @param program[]: program to run
void run_vm(uint256_t program[]);

// set a buffer to zero
// @param buffer: the buffer to clear
// @param start: the starting index to set to zero
// @param end: the ending index to set to zero
void clear_buffer(uint256_t buffer[], int start, int end);

#endif