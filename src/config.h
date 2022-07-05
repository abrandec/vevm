#ifndef CONFIG_H
#define CONFIG_H

/* Processor */
#define MAX_BYTECODE_LEN 96
// 24.576KB/256 bits = 96 indices (EIP-170:
// https://github.com/ethereum/EIPs/blob/master/EIPS/eip-170.md)
#define MAX_MEMORY_LEN 1000000 // 32 MB/256 bits = 1,000,000 indices
#define GAS 0xFFFFFFFFFFFFFFFF //
#define MAX_PC 3072            //

/* Stack */
#define MAX_STACK_DEPTH 1024

#endif