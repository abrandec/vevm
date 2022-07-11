#include "vm.h"

#include "../../common/math/bigint/bigint.h"
#include "../../errors/errors.h"
#include "../config.h"
#include "../opcodes/gas_table.h"
#include "../stack/stack.h"

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
// @param: opcode: dest to write opcode to
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

// EVM ADD operation
void _add(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  add_uint256(&a, &b, &a);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   MUL                         │
  └───────────────────────────────┘
 */

// EVM MUL operation
void _mul(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  mul_uint256(&a, &a, &b);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   SUB                         │
  └───────────────────────────────┘
 */

// EVM SUB operation
void _sub(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  sub_uint256(&a, &a, &b);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   DIV                         │
  └───────────────────────────────┘
 */

// EMV DIV operation
void _div(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c;
  uint256_t d;
  // just need the division
  divmod_uint256(&c, &d, &a, &b);

  stack_push(&c);
}

/*
  ┌───────────────────────────────┐
  │   MOD                         │
  └───────────────────────────────┘
 */

// EVM MOD operation
void _mod(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c;
  uint256_t d;

  // just need the modulus
  divmod_uint256(&c, &d, &a, &b);

  stack_push(&d);
}

/*
  ┌───────────────────────────────┐
  │   ADDMOD                      │
  └───────────────────────────────┘
 */

// EVM ADDMOD operation
void _addmod(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c = stack_peak(stack_length() - 1);
  stack_pop();

  // to deal with the possibility of values being written to before
  // arthemetic operations are complete
  uint256_t d;
  uint256_t e;

  // (a + b) % c
  add_uint256(&a, &a, &b);
  divmod_uint256(&e, &d, &a, &c);

  stack_push(&d);
}

/*
  ┌───────────────────────────────┐
  │   MULMOD                      │
  └───────────────────────────────┘
 */

// EVM MULMOD operation
void _mulmod(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c = stack_peak(stack_length() - 1);
  stack_pop();

  // to deal with the possibility of values being written to before
  // arthemetic operations are complete
  uint256_t d;
  uint256_t e;

  // (a * b) % c
  mul_uint256(&a, &a, &b);
  divmod_uint256(&e, &d, &a, &c);

  stack_push(&d);
}

/*
  ┌───────────────────────────────┐
  │   LT                          │
  └───────────────────────────────┘
 */

// EVM LT operation
void _lt(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  lt_uint256(&a, &b) ? change_all_uint256(&a, 0,0,0,1) : change_all_uint256(&a, 0,0,0,0);

  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   GT                          │
  └───────────────────────────────┘
 */

// EVM GT operation
void _gt(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();


  gt_uint256(&a, &b) ? change_all_uint256(&a, 0, 0, 0, 1) : change_all_uint256(&a, 0, 0, 0, 0);

  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   EQ                          │
  └───────────────────────────────┘
 */

// EVM EQ operation
void _eq(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  equal_uint256(&a, &b) ? change_all_uint256(&a, 0, 0, 0, 1) : change_all_uint256(&a, 0, 0, 0, 0);
  
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   ISZERO                      │
  └───────────────────────────────┘
 */

// EVM ISZERO operation
void _iszero(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = init_uint256(0);

  equal_uint256(&a, &b) ? change_all_uint256(&a, 0, 0, 0, 1) : change_all_uint256(&a, 0, 0, 0, 0);

  stack_push(&b);
}

/*
  ┌───────────────────────────────┐
  │   AND                         │
  └───────────────────────────────┘
 */

// EVM AND operation
void _and(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  and_uint256(&a, &a, &b);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   OR                          │
  └───────────────────────────────┘
 */

// EVM OR operation
void _or(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  or_uint256(&a, &a, &b);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   XOR                         │
  └───────────────────────────────┘
 */

// EVM XOR operation
void _xor(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  xor_uint256(&a, &a, &b);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   NOT                         │
  └───────────────────────────────┘
 */

// EVM NOT operation
void _not(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  not_uint256(&a, &a);

  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   SHL                         │
  └───────────────────────────────┘
 */

// EVM SHL operation
void _shl(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c = init_all_uint256(0, 0, 0, 0x00000000000000FF);

  if (gt_uint256(&a, &c)) {
    change_all_uint256(&c, 0, 0, 0, 0);
  } else {
    lshift_uint256(&c, &b, E11(a));
  }

  stack_push( &c);
}

/*
  ┌───────────────────────────────┐
  │   SHR                         │
  └───────────────────────────────┘
 */

// EVM SHR operation
void _shr(void) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();
  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c = init_all_uint256(0, 0, 0, 0x00000000000000FF);

  if (gt_uint256(&a, &c)) {
    change_all_uint256(&c, 0, 0, 0, 0);
  } else {
    rshift_uint256(&c, &b, E11(a));
  }

  stack_push(&c);
}

/*
  ┌───────────────────────────────┐
  │   Gas Limit                   │
  └───────────────────────────────┘
 */

// EVM GASLIMIT operation
void _gaslimit(void) {
  uint256_t a = init_all_uint256(0, 0, 0, GAS);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   MLOAD                       │
  └───────────────────────────────┘
 */

// EVM MLOAD operation
// @param memory[]: the memory
// @param mem_end: ending index of current memory usage
// @param mem_expanded: initial memory expansion check
// @param gas: gas meter
void _mload(uint256_t *memory, uint64_t *mem_end,
            bool *mem_expanded, uint64_t *gas) {}

/*
  ┌───────────────────────────────┐
  │   MSTORE                      │
  └───────────────────────────────┘
 */

// EVM MSTORE operation
// @param memory[]: the memory
// @param mem_end: ending index of current memory usage
// @param mem_expanded: initial memory expansion check
// @param gas: gas meter
void _mstore(uint256_t memory[], uint64_t *mem_end,
             bool *mem_expanded, uint64_t *gas) {
  // mask for 1st & 2nd index
  uint256_t mask1 = init_uint256(0xFFFFFFFFFFFFFFFF);
  uint256_t mask2 = init_uint256(0xFFFFFFFFFFFFFFFF);

  // POP and temporarily store values

  // data to push
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  // used to calculate the index of the memory & offset
  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  // (max mem length - 1) (999,999)
  uint256_t max_mem_len = init_all_uint256(0, 0, 0, 999999);

  uint64_t index = E11(a) / 32;
  uint64_t index2 = index + 1;
  uint64_t ending_index = index;
  uint64_t offset = ((E11(a)) % 32) * 8;

  // memory bounds check
  gt_uint256(&a, &max_mem_len) ? custom_error(VM_MEMORY_SIZE_EXCEEDED) : 0;

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

  uint512_t temp = init_uint512(0);

  // add the data from b onto the first index in the uint512_t
  E0(temp) = b;

  // shr data thats in the uint512_t
  rshift_uint512(&temp, &temp, offset);

  // prepare the masks
  lshift_uint256(&mask1, &mask1, 256 - offset);
  rshift_uint256(&mask2, &mask2, offset);

  // mask off a word in memory with both masks
  and_uint256(&memory[index], &memory[index], &mask1);
  and_uint256(&memory[index2], &memory[index2], &mask2);

  // combine the data from temp with memory in the 1st and 2nd index
  or_uint256(&memory[index], &memory[index], &E0(temp));
  or_uint256(&memory[index2], &memory[index2], &E1(temp));
}

/*
  ┌───────────────────────────────┐
  │   MSTORE8                     │
  └───────────────────────────────┘
 */

// EVM MSTORE8 operation
// @param memory[]: the memory
// @param mem_end: ending index of current memory usage
// @param mem_expanded: initial memory expansion check
// @param gas: gas left
void _mstore8(uint256_t memory[], uint64_t *mem_end,
              bool *mem_expanded, uint64_t *gas) {
  // masks for extracting the byte
  uint256_t mask1 = init_uint256(0xFFFFFFFFFFFFFFFF);
  uint256_t mask2 = init_uint256(0xFFFFFFFFFFFFFFFF);

  // POP and temporarily store values

  // data to push
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  // used to calculate the index of the memory & offset
  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  // (max mem length - 1) (999,999)
  uint256_t max_mem_len = init_all_uint256(0, 0, 0, 999999);

  uint64_t begin_index = E11(a) / 32;
  uint64_t end_index = begin_index + 1;
  uint64_t ending_index = begin_index;
  uint64_t offset = ((E11(a)) % 32) * 8;

  // memory bounds check
  gt_uint256(&a, &max_mem_len) ? custom_error(VM_MEMORY_SIZE_EXCEEDED) : 0;

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

  // invert mask to extract the byte from b
  not_uint256(&mask1, &mask1);

  // shift byte from b to correct position and mask off unwanted bits
  // e.g. offset = 16
  // mask1 = 0x00FF000000000000000000000000000000000000000000000000000000000000
  //     b = 0x0069000000000000000000000000000000000000000000000000000000000000
  lshift_uint256(&b, &b, 248 - offset);
  and_uint256(&b, &b, &mask1);

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

// EVM PC operation
// @param pc: the program counter
void _pc(int *pc) {
  uint256_t a = init_all_uint256(0, 0, 0, *pc - 1);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   MSIZE                       │
  └───────────────────────────────┘
 */

// EVM MSIZE operation
// @param mem_end: ending index of current memory usage
void _msize(uint64_t *mem_end) {
  uint256_t a = init_all_uint256(0, 0, 0, *mem_end * 8);

  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   Gas                         │
  └───────────────────────────────┘
 */

// EVM GAS operation
// @param gas: gas left
void _gas(uint64_t *gas) {
  uint256_t a = init_all_uint256(0, 0, 0, *gas);

  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   PUSH                        │
  └───────────────────────────────┘
 */

// EVM PUSH operation
// get bytes from program[] to push onto the stack
// @param program: the program[] to extract bytes from
// @param opcode: the push(x) opcode
// @param pc: the program counter
// @return bytes from program[] to push onto the stack
void _push(uint256_t program[], uint64_t *opcode, int *pc) {

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

  stack_push(&data1);
}

/*
  ┌───────────────────────────────┐
  │   DUP                         │
  └───────────────────────────────┘
 */

// EVM DUP operation
// @param opcode: the dup(x) opcode
void _dup(uint64_t *opcode) {
  // dup opcodes start @ 128 (0x80)
  int dup_index = *opcode - 127;

  uint256_t a = stack_peak(stack_length() - dup_index);

  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   SWAP                        │
  └───────────────────────────────┘
 */

// EVM SWAP operation
// @param opcode: the swap(x) opcode
void _swap(uint64_t *opcode) {
  // swap opcodes start @ 144 (0x90)
  int swap_index = *opcode - 143;

  stack_swap(swap_index);
}

/*
  ┌───────────────────────────────┐
  │   MISC                        │
  └───────────────────────────────┘
 */

void consume_gas(int *opcode, uint64_t *gas) {}

// set a buffer to zero
// @param buffer: the buffer to clear
// @param start: the starting index to set to zero
// @param end: the ending index to set to zero
void clear_buffer(uint256_t buffer[], int start, int end) {
  // foot shooty way to clear a buffer
  for (; start < end; ++start) {
    clear_uint256(&buffer[start]);
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

  // for storing current opcode
  uint64_t opcode;

  // for storing data to push onto stack
  uint256_t push_data;

  // while loop to run program //
  while (pc < MAX_PC) {
    get_opcode(program, &pc, &opcode);
#ifdef DEBUG
    // DEBUG MODE
    if (debug_mode) {
      
    }
#endif
    // consume_gas(&opcode, &gas);
    pc += 1;

    switch (opcode) {
    case 0x00: // STOP
      exit(1);
      break;
    case 0x01: // ADD
      _add();
      break;
    case 0x02: // MUL
      _mul();
      break;
    case 0x03: // SUB
      _sub();
      break;
    case 0x04: // DIV
      _div();
      break;
    case 0x06: // MOD
      _mod();
      break;
    case 0x08: // ADDMOD
      _addmod();
      break;
    case 0x09: // MULMOD
      _mulmod();
      break;
    case 0x10: // LT
      _lt();
      break;
    case 0x11: // GT
      _gt();
      break;
    case 0x14: // EQ
      _eq();
      break;
    case 0x15: // ISZERO
      _iszero();
      break;
    case 0x16: // AND
      _and();
      break;
    case 0x17: // OR
      _or();
      break;
    case 0x18: // XOR
      _xor();
      break;
    case 0x19: // NOT
      _not();
      break;
    case 0x1B: // SHL
      _shl();
      break;
    case 0x1C: // SHR
      _shr();
      break;
    case 0x45: // GASLIMIT
      _gaslimit();
      break;
    case 0x50: // POP
      stack_pop();
    case 0x51: // MLOAD
      _mload(memory, &mem_end, &mem_expanded, &gas);
      break;
    case 0x52: // MSTORE
      _mstore(memory, &mem_end, &mem_expanded, &gas);
      break;
    case 0x53: // MSTORE8
      _mstore8(memory, &mem_end, &mem_expanded, &gas);
      break;
    case 0x59: // MSIZE
      _msize(&mem_end);
      break;
    case 0x58: // PC
      _pc(&pc);
      break;
    case 0x5A: // GAS
      _gas(&gas);
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
      _push(program, &opcode, &pc);
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
      _dup(&opcode);
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
      _swap(&opcode);
      break;
    case 0xFE: // INVALID
      custom_error(VM_INVALID_OPCODE);
      break;
    case 0xFF: // SELFDESTRUCT
      exit(1);
      break;
    default:
      custom_error(VM_INVALID_OPCODE);
      break;
    }
  }
}