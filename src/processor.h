#ifndef processor_H_
#define processor_H_

// for running the EVM
// @param program[]: program to run
// @param DEBUG: whether to print debug messages
void vm(uint256_t program[], bool *DEBUG);

#endif