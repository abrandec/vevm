#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../bigint/bigint.h"
#include "../config.h"

#include <inttypes.h>



// for running the EVM
// @param program[]: program to run
// @param DEBUG: whether to print debug messages
void vm(uint256_t program[]);

// set a buffer to zero
// @param buffer: the buffer to initialize to zero
// @param length: the length of the buffer
void clear_buffer(uint256_t buffer[], int length);

#endif