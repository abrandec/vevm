#include "processor.h"
#include "bigint.h"
#include "debug.h"
#include "stack.h"
#include "utils/opcode_names.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

// XOR operation
void _xor(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  xor_uint256(&a, &a, &b);
  stack_push(stack, &a);
}

// NOT operation
void _not(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  not_uint256(&a, &a);

  stack_push(stack, &a);
}

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

// gaslimit operation
// @param stack: the stack
void _gaslimit(List *stack) {
  uint256_t a = init_all_uint256(0, 0, 0, GAS);
  stack_push(stack, &a);
}

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

  // check if the memory location >32MB
  if (gt_uint256(&a, &max_mem_len)) {
    char err[50] = "MEMORY SIZE EXCEEDED (>32MB)\n";
    custom_error(err);
  }

  //              gas cost stuff              //

  *gas -= 3;

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

// gas operation
// @param stack: the stack
// @param gas: gas left
void _gas(List *stack, uint64_t *gas) {
  uint256_t a = init_all_uint256(0, 0, 0, *gas);

  stack_push(stack, &a);
}

// get bytes from program[] to push onto the stack
// @param stack: the stack
// @param program: the program[] to extract bytes from
// @param opcode: the push(x) opcode
// @param pc: the program counter
// @return bytes from program[] to push onto the stack
void push_bytes(List *stack, uint256_t program[], uint64_t *opcode, int *pc) {

  // masks for getting data from program[] to push
  uint256_t mask256_1 = init_uint256(0xFFFFFFFFFFFFFFFF);
  uint256_t mask256_2 = init_uint256(0xFFFFFFFFFFFFFFFF);

  // place to store the data taken from program[] to push
  uint256_t data1 = init_uint256(0);
  uint256_t data2 = init_uint256(0);

  // calculate bytes to push from program (PUSH1 starts at 96)
  int num_bytes2push = *opcode - 95;

  // keep track of where to start pushing bytes from in program[]
  int prev_pc = *pc + 1;

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

// duplicate elements on the stack
// @param stack: the stack
// @param opcode: the dup(x) opcode
void _dup(List *stack, uint64_t *opcode) {
  // dup opcodes start @ 128 (0x80)
  int dup_index = *opcode - 127;

  uint256_t a = stack_peak(stack, stack_length(stack) - dup_index);

  stack_push(stack, &a);
}

// swap two elements from the stack
// @param stack: the stack
// @param opcode: the swap(x) opcode
void _swap(List *stack, uint64_t *opcode) {
  // swap opcodes start @ 144 (0x90)
  int swap_index = *opcode - 143;

  stack_swap(stack, swap_index);
}

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

// vroom vroom vroom vroom vroom vroom vroom vroom vroom vroom vroom vroom
// for running the EVM
// @param program[]: program to run
// @param DEBUG: whether to print debug messages
void vm(uint256_t program[], bool *DEBUG) {

  //      variables      //

  // EVM memory
  static uint256_t memory[MAX_MEMORY_LEN];

  char invalid_op_err[50] = "EVM - INVALID OPCODE\n";

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

  //      the action      //

  // clear all bits in buffers
  clear_buffer(memory, MAX_MEMORY_LEN);

  // while loop to run program //
  while (pc < MAX_PC) {
    get_opcode(program, &pc, &opcode);

    if (*DEBUG) {
      print_debug(stack, memory, &pc, &gas, &opcode);
    }

    // printf("GAS LEFT: %lld\n", gas);
    switch (opcode) {
    case 0x00: // STOP
      stack_destroy(stack);
      exit(1);
      break;

    case 0x01: // ADD
      gas -= 3;
      pc += 1;
      _add(stack);
      break;

    case 0x02: // MUL
      gas -= 5;
      pc += 1;

      _mul(stack);
      break;
    case 0x03: // SUB
      gas -= 3;
      pc += 1;
      _sub(stack);
      break;

    case 0x10: // LT
      gas -= 3;
      pc += 1;
      _lt(stack);
      break;
    case 0x11: // GT
      gas -= 3;
      pc += 1;
      _gt(stack);
      break;

    case 0x14: // EQ
      gas -= 3;
      pc += 1;
      _eq(stack);
      break;

    case 0x15: // ISZERO
      gas -= 3;
      pc += 1;
      _iszero(stack);
      break;

    case 0x16: // AND
      gas -= 3;
      pc += 1;
      _and(stack);
      break;

    case 0x17: // OR
      gas -= 3;
      pc += 1;
      _or(stack);
      break;

    case 0x18: // XOR
      gas -= 3;
      pc += 1;
      _xor(stack);
      break;

    case 0x19: // NOT
      gas -= 3;
      pc += 1;
      _not(stack);
      break;

    case 0x1B: // SHL
      gas -= 3;
      pc += 1;
      _shl(stack);
      break;

    case 0x1C: // SHR
      gas -= 3;
      pc += 1;
      _shr(stack);
      break;

    case 0x45: // GASLIMIT
      gas -= 2;
      pc += 1;
      _gaslimit(stack);
      break;

    case 0x50: // POP
      gas -= 2;
      pc += 1;
      stack_pop(stack);
      break;

    case 0x52: // MSTORE
      pc += 1;
      // gas calculated in _mstore
      _mstore(stack, memory, &mem_end, &mem_expanded, &gas);
      break;

    case 0x58: // PC
      gas -= 2;
      uint256_t prev_pc = init_all_uint256(0, 0, 0, pc);
      stack_push(stack, &prev_pc);
      pc += 1;
      break;

    case 0x5A: // GAS
      gas -= 2;
      pc += 1;
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
      gas -= 3;
      // pc updated in push_bytes()
      push_bytes(stack, program, &opcode, &pc);
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
      gas -= 3;
      pc += 1;
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
      gas -= 3;
      pc += 1;
      _swap(stack, &opcode);
      break;

    case 0xFE: // INVALID
      pc = MAX_PC + 1;
      custom_error(invalid_op_err);
      break;

    case 0xFF: // SELFDESTRUCT
      stack_destroy(stack);
      exit(1);
      break;
    }
  }
}