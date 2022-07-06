#include "../../common/math/bigint/bigint.h"
#include "../../errors/errors.h"
#include "../config.h"
#include "../opcodes/gas_table.h"
#include "../stack/stack.h"

#include "vm.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef DEBUG
#include "../../debug/debug.h"
#endif

/*
  ┌───────────────────────────────┐
  │   GET OPCODE                  │
  └───────────────────────────────┘
 */

// Get opcode from program buffer
// @param program[]: program buffer
// @param pc: program counter
// @param: opcode: dest to write to
void get_opcode(uint256_t program[], int *pc, uint64_t *opcode) {
  // opcode mask
  uint64_t op_mask = 0xFF00000000000000;
  // get uint256_t index by dividing by 32 (32 bytes in a uint256_t)
  int index = *pc / 32;
  // get location of opcode within the uint256_t
  int offset = *pc % 32;
  // amount to shift the mask by to get the opcode
  int op_mask_mv = ((offset % 8) * 8);

  // shift the mask to extract an opcode within a uint64_t
  // e.g. 0xFF00000000000000 >> (2 * 8) = 0x0000FF000000000
  op_mask = op_mask >> op_mask_mv;

  // get uint64_t that opcode is located in
  switch (offset / 8) {
  case 0: // uint256_t[index].uint128_t[0].uint64_t[0]
    *opcode = E00(program[index]);
    break;
  case 1: // uint256_t[index].uint128_t[0].uint64_t[1]
    *opcode = E01(program[index]);
    break;
  case 2: // uint256_t[index].uint128_t[1].uint64_t[0]
    *opcode = E10(program[index]);
    break;
  case 3: // uint256_t[index].uint128_t[1].uint64_t[1]
    *opcode = E11(program[index]);
    break;
  }

  // 1. get opcode by ANDing the opcode with the mask
  // e.g. 0x602261694201611 & 0x0000FF000000000 = 0x000061000000000
  // 2. shift opcode to the right to get the actual value
  // e.g. 0x000061000000000 -> 0x000000000000061
  // write to dest
  *opcode = (*opcode & op_mask) >> (56 - op_mask_mv);
}

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   OPCODES                                                                  │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   ADD                         │
  └───────────────────────────────┘
 */

// Add operation
// @param stack: the stack
void _add(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  add_uint256(&a, &a, &b);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   MUL                         │
  └───────────────────────────────┘
 */

// multiplication operation
// @param stack: the stack
void _mul(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  mul_uint256(&a, &a, &b);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   SUB                         │
  └───────────────────────────────┘
 */

// Subtract operation
// @param stack: the stack
void _sub(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  sub_uint256(&a, &a, &b);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   LT                          │
  └───────────────────────────────┘
 */

// Less than operation
// @param stack: the stack
void _lt(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  if (lt_uint256(&a, &b)) {
    change_uint256(&a, 0, 0, 0, 1);
  } else {
    change_uint256(&a, 0, 0, 0, 0);
  }

  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   GT                          │
  └───────────────────────────────┘
 */

// Greater than operation
// @param stack: the stack
void _gt(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  if (gt_uint256(&a, &b)) {
    change_uint256(&a, 0, 0, 0, 1);
  } else {
    change_uint256(&a, 0, 0, 0, 0);
  }

  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   EQ                          │
  └───────────────────────────────┘
 */

// Equal operation
// @param stack: the stack
void _eq(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  if (equal_uint256(&a, &b)) {
    change_uint256(&a, 0, 0, 0, 1);
  } else {
    change_uint256(&a, 0, 0, 0, 0);
  }

  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   ISZERO                      │
  └───────────────────────────────┘
 */

// iszero operation
// @param stack: the stack
void _iszero(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = init_uint256(0);

  if (equal_uint256(&a, &b)) {
    change_uint256(&b, 0, 0, 0, 1);
  }

  stack_push(stack, &b);
}

/*
  ┌───────────────────────────────┐
  │   AND                         │
  └───────────────────────────────┘
 */

// AND operation
// @param stack: the stack
void _and(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  and_uint256(&a, &a, &b);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   OR                          │
  └───────────────────────────────┘
 */

// OR operation
// @param stack: the stack
void _or(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  or_uint256(&a, &a, &b);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   XOR                         │
  └───────────────────────────────┘
 */

// XOR operation
// @param stack: the stack
void _xor(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  xor_uint256(&a, &a, &b);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   NOT                         │
  └───────────────────────────────┘
 */

// NOT operation
// @param stack: the stack
void _not(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  not_uint256(&a, &a);

  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   SHL                         │
  └───────────────────────────────┘
 */

// Left shift operation
// @param stack: the stack
void _shl(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t c = init_all_uint256(0, 0, 0, 0x00000000000000FF);

  if (gt_uint256(&a, &c)) {
    change_uint256(&c, 0, 0, 0, 0);
  } else {
    lshift_uint256(&c, &b, E11(a));
  }

  stack_push(stack, &c);
}

/*
  ┌───────────────────────────────┐
  │   SHR                         │
  └───────────────────────────────┘
 */

// Right shift operation
// @param stack: the stack
void _shr(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t c = init_all_uint256(0, 0, 0, 0x00000000000000FF);

  if (gt_uint256(&a, &c)) {
    change_uint256(&c, 0, 0, 0, 0);
  } else {
    rshift_uint256(&c, &b, E11(a));
  }

  stack_push(stack, &c);
}

/*
  ┌───────────────────────────────┐
  │   Gas Limit                   │
  └───────────────────────────────┘
 */

// gaslimit operation
// @param stack: the stack
void _gaslimit(List *stack) {
  uint256_t a = init_all_uint256(0, 0, 0, GAS);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   MSTORE                      │
  └───────────────────────────────┘
 */

// mstore operation
// @param stack: the stack
// @param memory[]: the memory
// @param mem_end: ending index of current memory usage
// @param mem_expanded: initial memory expansion check
// @param gas: gas left
void _mstore(List *stack, uint256_t memory[], uint64_t *mem_end,
             bool *mem_expanded, uint64_t *gas) {
  // mask for 1st & 2nd index
  uint256_t mask1 = init_uint256(0xFFFFFFFFFFFFFFFF);
  uint256_t mask2 = init_uint256(0xFFFFFFFFFFFFFFFF);

  // POP and temporarily store values

  // data to push
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  // used to calculate the index of the memory & offset
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  // (max mem length - 1) in bytes (999,999)
  uint256_t max_mem_len = init_all_uint256(0, 0, 0, 0x00000000000F423F);

  uint64_t index = E11(a) / 32;
  uint64_t index2 = index + 1;
  uint64_t ending_index = index;
  uint64_t offset = ((E11(a)) % 32) * 8;

  // error checking
  if (gt_uint256(&a, &max_mem_len)) {
    custom_error(memory_size_exceeded_err);
  }

  //              gas cost stuff              //

  // check if memory has been expanded before
  if (*mem_expanded) {
    *mem_expanded = true;
    *gas -= 3;
  }

  switch (offset) {
  case 0:
    break;
  default:
    ending_index += 1;
    if (ending_index - *mem_end > 0) {
      *gas -= ending_index * 3;
      *mem_end = ending_index;
    }
    break;
  }

  //              bitmasking stuff            //

  uint512_t temp = init_uint512(0);

  E0(temp) = b;

  rshift_uint512(&temp, &temp, offset);

  lshift_uint256(&mask1, &mask1, 256 - offset);
  rshift_uint256(&mask2, &mask2, offset);

  and_uint256(&memory[index], &memory[index], &mask1);
  and_uint256(&memory[index2], &memory[index2], &mask2);

  or_uint256(&memory[index], &memory[index], &E0(temp));
  or_uint256(&memory[index2], &memory[index2], &E1(temp));
}

/*
  ┌───────────────────────────────┐
  │   MSTORE8                     │
  └───────────────────────────────┘
 */

// mstore8 operation
// @param stack: the stack
// @param memory[]: the memory
// @param mem_end: ending index of current memory usage
// @param mem_expanded: initial memory expansion check
// @param gas: gas left
void _mstore8(List *stack, uint256_t memory[], uint64_t *mem_end,
              bool *mem_expanded, uint64_t *gas) {
  // masks for extracting the byte
  uint256_t mask1 = init_uint256(0xFFFFFFFFFFFFFFFF);
  uint256_t mask2 = init_uint256(0xFFFFFFFFFFFFFFFF);

  // POP and temporarily store values

  // data to push
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  // used to calculate the index of the memory & offset
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  // (max mem length - 1) in bytes (999,999)
  uint256_t max_mem_len = init_all_uint256(0, 0, 0, 0x00000000000F423F);

  uint64_t begin_index = E11(a) / 32;
  uint64_t end_index = begin_index + 1;
  uint64_t ending_index = begin_index;
  uint64_t offset = ((E11(a)) % 32) * 8;

  // error checking
  if (gt_uint256(&a, &max_mem_len)) {
    custom_error(memory_size_exceeded_err);
  }

  //              gas cost stuff              //

  // check if memory has been expanded before
  if (*mem_expanded) {
    *mem_expanded = true;
    *gas -= 3;
  }

  switch (offset) {
  case 0:
    break;
  default:
    ending_index += 1;
    if (ending_index - *mem_end > 0) {
      *gas -= ending_index * 3;
      *mem_end = ending_index;
    }
    break;
  }

  //              bitshifting stuff            //

  // shift masks to extract a byte
  // e.g. offset = 16
  // mask1 = 0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
  // mask2 = 0xFF00000000000000000000000000000000000000000000000000000000000000
  rshift_uint256(&mask1, &mask1, offset + 8);
  lshift_uint256(&mask2, &mask2, 256 - offset);

  // combine masks
  // e.g. offset = 16
  // mask1 = 0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
  // mask2 = 0xFF00000000000000000000000000000000000000000000000000000000000000
  // OR
  // mask1 = 0xFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
  or_uint256(&mask1, &mask1, &mask2);

  // mask-off single byte from memory using the combined masks
  // e.g. offset = 16
  // mask1 = 0xFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
  // memory[begin_index] =
  // 0x333333333333333333333333333333333333333333333333333333333333333 AND
  // memory[begin_index] =
  // 0x3300333333333333333333333333333333333333333333333333333333333333
  and_uint256(&memory[begin_index], &memory[begin_index], &mask1);

  // shift byte from b to correct position
  // e.g. offset = 16
  // b = 0x0069000000000000000000000000000000000000000000000000000000000000
  lshift_uint256(&b, &b, 248 - offset);

  // combine byte from b with memory
  // e.g. offset = 16
  // memory[begin_index] =
  // 0x3300333333333333333333333333333333333333333333333333333333333333 b =
  // 0x0069000000000000000000000000000000000000000000000000000000000000 OR
  // memory[begin_index] =
  // 0x3369333333333333333333333333333333333333333333333333333333333333
  or_uint256(&memory[begin_index], &memory[begin_index], &b);
}

/*
  ┌───────────────────────────────┐
  │   PC                          │
  └───────────────────────────────┘
 */

// pc operation
// @param stack: the stack
// @param pc: the program counter
void _pc(List *stack, int *pc) {
  uint256_t a = init_all_uint256(0, 0, 0, *pc - 1);
  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   MSIZE                       │
  └───────────────────────────────┘
 */

// msize operation
// @param stack: the stack
// @param mem_end: ending index of current memory usage
void _msize(List *stack, uint64_t *mem_end) {
  uint256_t a = init_all_uint256(0, 0, 0, *mem_end * 8);

  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   Gas                         │
  └───────────────────────────────┘
 */

// gas operation
// @param stack: the stack
// @param gas: gas left
void _gas(List *stack, uint64_t *gas) {
  uint256_t a = init_all_uint256(0, 0, 0, *gas);

  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   PUSH                        │
  └───────────────────────────────┘
 */

// get bytes from program[] to push onto the stack
// @param stack: the stack
// @param program: the program[] to extract bytes from
// @param opcode: the push(x) opcode
// @param pc: the program counter
// @return bytes from program[] to push onto the stack
void _push(List *stack, uint256_t program[], uint64_t *opcode, int *pc) {

  // masks for getting data from program[] to push
  uint256_t mask256_1 = init_uint256(0xFFFFFFFFFFFFFFFF);
  uint256_t mask256_2 = init_uint256(0xFFFFFFFFFFFFFFFF);

  // place to store the data taken from program[] to push
  uint256_t data1 = init_uint256(0);
  uint256_t data2 = init_uint256(0);

  // calculate bytes to push from program (PUSH1 starts at 96)
  int num_bytes2push = *opcode - 95;

  // keep track of where to start pushing bytes from in program[]
  int prev_pc = *pc;

  // increment program counter to next instruction
  *pc = prev_pc + num_bytes2push;

  // the index of the first byte to push
  int starting_index = prev_pc / 32;

  // the index of the last bytes to push
  int ending_index = (*pc - 1) / 32;

  // only need to get data from one uint256_t in program[]
  if (starting_index == ending_index) {

    // left shift mask just in case we're extracting bytes that don't reach the
    // end of the program[starting_index]
    lshift_uint256(&mask256_1, &mask256_1, (32 - num_bytes2push) * 8);

    // right shift mask to get the data to extract from program[starting_index]
    rshift_uint256(&mask256_1, &mask256_1, (prev_pc % 32) * 8);

    // extract the bytes from program[starting_index]
    and_uint256(&data1, &program[starting_index], &mask256_1);

    // shift data1 to the right to get the actual value
    rshift_uint256(&data1, &data1, (31 - ((*pc - 1) % 32)) * 8);

    // else get data from two uint256_ts in program[]
  } else {

    // first index //
    rshift_uint256(&mask256_1, &mask256_1, (prev_pc % 32) * 8);

    and_uint256(&data1, &program[starting_index], &mask256_1);

    lshift_uint256(&data1, &data1, 256 - (32 - (*pc % 32)) * 8);

    // second index //
    lshift_uint256(&mask256_2, &mask256_2, (32 - (*pc % 32)) * 8);

    and_uint256(&data2, &program[ending_index], &mask256_2);

    rshift_uint256(&data2, &data2, (31 - ((*pc - 1) % 32)) * 8);

    // combine data1 and data2 //
    or_uint256(&data1, &data1, &data2);
  }

  stack_push(stack, &data1);
}

/*
  ┌───────────────────────────────┐
  │   DUP                         │
  └───────────────────────────────┘
 */

// duplicate elements on the stack
// @param stack: the stack
// @param opcode: the dup(x) opcode
void _dup(List *stack, uint64_t *opcode) {
  // dup opcodes start @ 128 (0x80)
  int dup_index = *opcode - 127;

  uint256_t a = stack_peak(stack, stack_length(stack) - dup_index);

  stack_push(stack, &a);
}

/*
  ┌───────────────────────────────┐
  │   SWAP                        │
  └───────────────────────────────┘
 */

// swap two elements from the stack
// @param stack: the stack
// @param opcode: the swap(x) opcode
void _swap(List *stack, uint64_t *opcode) {
  // swap opcodes start @ 144 (0x90)
  int swap_index = *opcode - 143;

  stack_swap(stack, swap_index);
}

/*
  ┌───────────────────────────────┐
  │   MISC                        │
  └───────────────────────────────┘
 */

void consume_gas(int *opcode, uint64_t *gas) {}

// set a buffer to zero
// @param buffer: the buffer to initialize to zero
// @param length: the length of the buffer
void clear_buffer(uint256_t buffer[], int length) {
  int i = 0;
  length = length - 1;

  for (; i < length; ++i) {
    clear_uint256(&buffer[i]);
  }
}

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   VM                                                                       │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

// Entry point for EVM
// @param program[]: program to run
// @param debug_mode: whether to print debug messages
void _vm(uint256_t program[], bool debug_mode) {
  // printf("%d\n", GAS_TABLE[0]);
  //      variables      //

  // EVM memory
  static uint256_t memory[MAX_MEMORY_LEN];

  // for keeping track memory expansion costs
  uint64_t mem_end = 0;

  // for checking if the first index in memory is being used (gas calc)
  bool mem_expanded = false;

  // initialize program counter
  int pc = 0;

  uint64_t gas = GAS - 21000;

  // initialize stack
  List *stack = stack_create();

  // for storing current opcode
  uint64_t opcode;

  // for storing data to push onto stack
  uint256_t push_data;

  // clear all bits in memory
  clear_buffer(memory, MAX_MEMORY_LEN);
  change_uint256(&memory[0], 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);

  // while loop to run program //
  while (pc < MAX_PC) {
    get_opcode(program, &pc, &opcode);

    /*
    ┌────────────────────┐
    │   DEBUG            │
    └────────────────────┘
    */
    if (debug_mode) {
      print_debug(stack, memory, &pc, &gas, &opcode);
    }

    // consume_gas(&opcode, &gas);
    pc += 1;

    switch (opcode) {
    case 0x00: // STOP
      stack_destroy(stack);
      exit(1);
      break;
    case 0x01: // ADD
      _add(stack);
      break;
    case 0x02: // MUL
      _mul(stack);
      break;
    case 0x03: // SUB
      _sub(stack);
      break;
    case 0x10: // LT
      _lt(stack);
      break;
    case 0x11: // GT
      _gt(stack);
      break;
    case 0x14: // EQ
      _eq(stack);
      break;
    case 0x15: // ISZERO
      _iszero(stack);
      break;
    case 0x16: // AND
      _and(stack);
      break;
    case 0x17: // OR
      _or(stack);
      break;
    case 0x18: // XOR
      _xor(stack);
      break;
    case 0x19: // NOT
      _not(stack);
      break;
    case 0x1B: // SHL
      _shl(stack);
      break;
    case 0x1C: // SHR
      _shr(stack);
      break;
    case 0x45: // GASLIMIT
      _gaslimit(stack);
      break;
    case 0x50: // POP
      stack_pop(stack);
      break;
    case 0x52: // MSTORE
      _mstore(stack, memory, &mem_end, &mem_expanded, &gas);
      break;
    case 0x53: // MSTORE8
      _mstore8(stack, memory, &mem_end, &mem_expanded, &gas);
      break;
    case 0x59: // MSIZE
      _msize(stack, &mem_end);
      break;
    case 0x58: // PC
      _pc(stack, &pc);
      break;
    case 0x5A: // GAS
      _gas(stack, &gas);
      break;
    case 0x60: // PUSH1
    case 0x61: // PUSH2
    case 0x62: // PUSH3
    case 0x63: // PUSH4
    case 0x64: // PUSH5
    case 0x65: // PUSH6
    case 0x66: // PUSH7
    case 0x67: // PUSH8
    case 0x68: // PUSH9
    case 0x69: // PUSH10
    case 0x6A: // PUSH11
    case 0x6B: // PUSH12
    case 0x6C: // PUSH13
    case 0x6D: // PUSH14
    case 0x6E: // PUSH15
    case 0x6F: // PUSH16
    case 0x70: // PUSH17
    case 0x71: // PUSH18
    case 0x72: // PUSH19
    case 0x73: // PUSH20
    case 0x74: // PUSH21
    case 0x75: // PUSH22
    case 0x76: // PUSH23
    case 0x77: // PUSH24
    case 0x78: // PUSH25
    case 0x79: // PUSH26
    case 0x7A: // PUSH27
    case 0x7B: // PUSH28
    case 0x7C: // PUSH29
    case 0x7D: // PUSH30
    case 0x7E: // PUSH31
    case 0x7F: // PUSH32
      _push(stack, program, &opcode, &pc);
      break;
    case 0x80: // DUP1
    case 0x81: // DUP2
    case 0x82: // DUP3
    case 0x83: // DUP4
    case 0x84: // DUP5
    case 0x85: // DUP6
    case 0x86: // DUP7
    case 0x87: // DUP8
    case 0x88: // DUP9
    case 0x89: // DUP10
    case 0x8A: // DUP11
    case 0x8B: // DUP12
    case 0x8C: // DUP13
    case 0x8D: // DUP14
    case 0x8E: // DUP15
    case 0x8F: // DUP16
      _dup(stack, &opcode);
      break;
    case 0x90: // SWAP1
    case 0x91: // SWAP2
    case 0x92: // SWAP3
    case 0x93: // SWAP4
    case 0x94: // SWAP5
    case 0x95: // SWAP6
    case 0x96: // SWAP7
    case 0x97: // SWAP8
    case 0x98: // SWAP9
    case 0x99: // SWAP10
    case 0x9A: // SWAP11
    case 0x9B: // SWAP12
    case 0x9C: // SWAP13
    case 0x9D: // SWAP14
    case 0x9E: // SWAP15
    case 0x9F: // SWAP16
      _swap(stack, &opcode);
      break;
    case 0xFE: // INVALID
      custom_error(invalid_op_err);
      break;
    case 0xFF: // SELFDESTRUCT
      exit(1);
      break;
    default:
      custom_error(invalid_op_err);
      break;
    }
  }
}