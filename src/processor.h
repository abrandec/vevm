#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "bigint.h"
#include "config.h"

#include <inttypes.h>
#include <stdbool.h>

// for running the EVM
// @param program[]: program to run
// @param DEBUG: whether to print debug messages
void vm(uint256_t program[], bool *DEBUG);

// set a buffer to zero
// @param buffer: the buffer to initialize to zero
// @param length: the length of the buffer
void clear_buffer(uint256_t static_buffer[], int length);

#endif