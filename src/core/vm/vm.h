#ifndef VM_H
#define VM_H

#include "../../common/math/bigint/bigint.h"
#include "../config.h"

#include <inttypes.h>

// Entry point for EVM
// @param program[]: program to run
// @param DEBUG: whether to print debug messages
void _vm(uint256_t program[]);

// set a buffer to zero
// @param buffer: the buffer to initialize to zero
// @param length: the length of the buffer
void clear_buffer(uint256_t buffer[], int length);

#endif