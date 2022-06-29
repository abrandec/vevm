
#include "h_stack.h"
#include "uint256.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BYTECODE_LEN 96    // 24.576KB/256 bits = 96 indices
#define MAX_MEMORY_LEN 1000000 // 32 MB/256 bits = 1000000 indices
#define GAS 0xFFFFFFFFFFFFFFFF //

// Get opcode from program buffer
// @param program[]: program buffer
// @param pc: program counter
// @param: opcode: dest to write to
void get_opcode(uint256_t program[], int *pc, uint64_t *opcode) {
  // opcodes are 1 byte, we just use FF and shift it to the right
  // since we're reading the program left to right
  uint64_t mask = 0xFF00000000000000;

  // get uint256_t index by dividing by 32 (32 bytes in a uint256_t)
  int index = *pc / 32;

  // get location of opcode within the uint256_t
  int offset = *pc % 32;

  // amount to shift the mask by to get the opcode
  int mask_mv = ((offset % 8) * 8);

  // shift the mask to extract an opcode within a uint64_t
  // e.g. 0xFF00000000000000 >> (2 * 8) = 0x0000FF000000000
  mask = mask >> mask_mv;

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
  *opcode = (*opcode & mask) >> (56 - mask_mv);
}

// Add operation
// @param stack: the stack
void _add(List *stack) {
  // 0x000 starts @ whatever the length of the stack is - 1
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);

  uint256_t c;

  add_uint256(&c, &a, &b);

  stack_push(stack, &c);
}

// Subtract operation
// @param stack: the stack
void _sub(List *stack) {
  // 0x000 starts @ whatever the length of the stack is - 1
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  print_hex_uint256(&a);
  printf("\n");
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  print_hex_uint256(&b);
  printf("\n");
  uint256_t c;

  sub_uint256(&c, &a, &b);
  print_hex_uint256(&c);
  stack_push(stack, &c);
}

void _lt(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t c = init_uint256(0);
  switch (lt_uint256(&a, &b)) {
  case true:
    c = set_uint256(0, 0, 0, 1);
    break;

  default:
    break;
  }
  stack_push(stack, &c);
}

void _gt(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t c = init_uint256(0);

  switch (gt_uint256(&a, &b)) {
  case true:
    c = set_uint256(0, 0, 0, 1);

    break;
  default:
    break;
  }
  stack_push(stack, &c);
}

// Equal operation
// @param stack: the stack
void _eq(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  print_hex_uint256(&a);
  printf("\n");
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  print_hex_uint256(&b);
  printf("\n");
  uint256_t c = init_uint256(0);
  switch (equal_uint256(&a, &b)) {

  case true:
    c = set_uint256(0, 0, 0, 1);
    stack_push(stack, &c);
    break;

  case false:
    stack_push(stack, &c);
    break;
  }
}

void _iszero(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t c = init_uint256(0);
  switch (equal_uint256(&a, &c)) {

  case true:
    c = set_uint256(0, 0, 0, 1);
    stack_push(stack, &c);
    break;

  case false:
    stack_push(stack, &c);
    break;
  }
}

// AND operation
// @param stack: the stack
void _and(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t b = stack_peak(stack, 0);
  stack_pop(stack);
  uint256_t c;

  and_uint256(&c, &a, &b);
  stack_push(stack, &c);
}

// OR operation
// @param stack: the stack
void _or(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t b = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t c;

  or_uint256(&c, &a, &b);
  stack_push(stack, &c);
}

// xor

//

// Left shift operation
// @param stack: the stack
void _shl(List *stack) {
  uint256_t a = stack_peak(stack, stack_length(stack) - 1);
  stack_pop(stack);
  uint256_t b = stack_peak(stack, 0);
  stack_pop(stack);

  uint256_t c = set_uint256(0, 0, 0, 0x00000000000000FF);

  switch (gt_uint256(&a, &c)) {

  case true:
    c = init_uint256(0);
    break;
  case false:
    lshift_uint256(&c, &b, E11(a));
    break;
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

  uint256_t c = set_uint256(0, 0, 0, 0x00000000000000FF);

  switch (gt_uint256(&a, &c)) {

  case true:
    c = init_uint256(0);
    break;
  case false:
    rshift_uint256(&c, &b, E11(a));
    break;
  }

  stack_push(stack, &c);
}

// gaslimit operation
// @param stack: the stack
void _gaslimit(List *stack) {
  uint256_t a = set_uint256(0, 0, 0, GAS);
  stack_push(stack, &a);
}

void _gas(List *stack, uint64_t *gas) {
  uint256_t a = set_uint256(0, 0, 0, *gas);
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

void _swap(List *stack, uint64_t *opcode) {
  // swap opcodes start @ 144 (0x90)
  int swap_index = *opcode - 143;
  printf("swap_index: %d\n", swap_index);
  stack_swap(stack, swap_index);
}

// set program buffer to zero
// @param program: the program buffer to initialize to zero
void clear_program(uint256_t program[]) {
  int i = 0;

  for (i = 0; i < MAX_BYTECODE_LEN - 1; ++i) {
    clear_uint256(&program[i]);
  }
}

// print functions //

// prints the entire program buffer
// @param program: the program buffer to print
void print_program(uint256_t program[]) {
  printf("\nPROGRAM\n----------------------------------------------------------"
         "------\n");
  for (int i = 0; i < MAX_BYTECODE_LEN - 1; ++i) {
    print_hex_uint256(&program[i]);
  }
  printf("----------------------------------------------------------------\n");
}

// prints the memory
// @param memory: the memory to print
void print_memory(uint256_t memory[]) {
  printf("\nMEMORY\n----------------------------------------------------------"
         "------\n");
  for (int i = 0; i < MAX_MEMORY_LEN - 1; ++i) {
    print_hex_uint256(&memory[i]);
  }
  printf("----------------------------------------------------------------\n");
}

// vroom vroom vroom vroom vroom vroom vroom vroom vroom vroom vroom vroom
int vm(void) {

  //      variables      //

  // program buffer
  // EIP-3670 (EOF check) cannot be done inside this function and must be
  // implemented beforehand
  uint256_t program[MAX_BYTECODE_LEN];

  // EVM memory
  uint256_t memory[MAX_MEMORY_LEN];

  // initialize program counter
  int pc = 0;

  uint64_t gas = GAS - 21000;

  // initialize stack
  List *stack = stack_create();

  // for storing current opcode
  uint64_t opcode = 0;

  // for storing data to push onto stack
  uint256_t push_data;

  //      the action      //

  // set all bits in program buffer to 0
  clear_program(program);

  // sample program
  program[0] = set_uint256(0x5A60016010609060, 0x1160126014950069,
                           0x8869696969696944, 0x6099633969697711);
  program[1] = set_uint256(0x6942026169420161, 0x3300000000000000, 0, 0);

  printf("EVM TEST\n");

  // while loop to run program //
  while (pc < 100) {

    get_opcode(program, &pc, &opcode);

    printf("OPCODE %02llX \n", opcode);
    // printf("GAS LEFT: %lld\n", gas);

    switch (opcode) {
    case 0x00: // STOP
      printf("STOP\n");
      stack_destroy(stack);
      exit(1);
      break;

    case 0x01: // ADD
      gas -= 3;
      pc += 1;
      _add(stack);
      break;

    case 0x02: // SUB
      gas -= 3;
      pc += 1;
      _sub(stack);
      stack_print(stack);
      break;

    case 0x10: // LT
      gas -= 3;
      pc += 1;
      _lt(stack);
      stack_print(stack);
      break;
    case 0x11: // GT
      gas -= 3;
      pc += 1;
      _gt(stack);
      stack_print(stack);
      break;

    case 0x14: // EQ
      gas -= 3;
      pc += 1;
      _eq(stack);
      stack_print(stack);
      break;

    case 0x15: // ISZERO
      gas -= 3;
      pc += 1;
      _iszero(stack);
      stack_print(stack);
      break;

    case 0x16: // AND
      gas -= 3;
      pc += 1;
      _and(stack);
      stack_print(stack);
      break;

    case 0x17: // OR
      gas -= 3;
      pc += 1;
      _or(stack);
      break;

    case 0x1B: // SHL
      gas -= 3;
      pc += 1;
      _shl(stack);
      stack_print(stack);
      break;

    case 0x1C: // SHR
      gas -= 3;
      pc += 1;
      _shr(stack);
      stack_print(stack);
      break;

    case 0x45: // GASLIMIT
      gas -= 2;
      pc += 1;
      _gaslimit(stack);
      stack_print(stack);
      break;

    case 0x50: // POP
      gas -= 2;
      pc += 1;
      stack_pop(stack);
      break;

    case 0x58: // PC
      gas -= 2;
      uint256_t prev_pc = set_uint256(0, 0, 0, pc);
      stack_push(stack, &prev_pc);
      pc += 1;
      break;

    case 0x5A: // GAS
      gas -= 2;
      pc += 1;
      _gas(stack, &gas);
      stack_print(stack);

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
      stack_print(stack);
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
      stack_peak_print(stack, stack_length(stack) - 1);
      _dup(stack, &opcode);
      stack_print(stack);
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
      stack_print(stack);
      break;

    case 0xFE: // INVALID
      printf("INVALID\n");
      stack_destroy(stack);
      exit(1);
      break;

    default:
      printf("UNKNOWN OPCODE\n");
      stack_destroy(stack);
      exit(1);
      break;
    }
  }
}