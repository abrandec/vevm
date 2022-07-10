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
// @param debug_mode: whether to print debug messages
void _vm(uint256_t program[], bool debug_mode);

// set a buffer to zero
// @param buffer: the buffer to clear
// @param start: the starting index to set to zero
// @param end: the ending index to set to zero
void clear_buffer(uint256_t buffer[], int start, int end);

#endif