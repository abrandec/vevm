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
// @param vm: vm state
void get_opcode(uint256_t program[], vm_state_t *vm) {
  uint256_t op_mask = init_all_uint256(0xFF00000000000000, 0, 0, 0);
  // index opcode is in
  int index = vm->pc / 32;

  // amount to shift the mask by to get the opcode
  int op_mask_mv = ((vm->pc % 32) * 8);

  rshift_uint256(&op_mask, &op_mask, op_mask_mv);
  and_uint256(&op_mask, &program[index], &op_mask);
  rshift_uint256(&op_mask, &op_mask, 248 - op_mask_mv);

  vm->opcode = E11(op_mask);
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
// @param vm: vm state
void _add(vm_state_t *vm) {
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
// @param vm_state: VM state
void _mul(vm_state_t *vm) {
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
// @param vm_state: VM state
void _sub(vm_state_t *vm) {
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
// @param vm_state: VM state
void _div(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c = init_uint256(0);
  uint256_t d;

  equal_uint256(&b, &c) ? 0 : divmod_uint256(&c, &d, &a, &b);
  stack_push(&c);
}

/*
  ┌───────────────────────────────┐
  │   MOD                         │
  └───────────────────────────────┘
 */

// EVM MOD operation
// @param vm_state: VM state
void _mod(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c;
  uint256_t d = init_uint256(0);

  equal_uint256(&b, &d) ? 0 : divmod_uint256(&c, &d, &a, &b);
  stack_push(&d);
}

/*
  ┌───────────────────────────────┐
  │   ADDMOD                      │
  └───────────────────────────────┘
 */

// EVM ADDMOD operation
// @param vm_state: VM state
void _addmod(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t d = init_uint256(0);
  uint256_t e;

  // (a + b) % c
  add_uint256(&a, &a, &b);
  equal_uint256(&c, &d) ? 0 : divmod_uint256(&e, &d, &a, &c);
  stack_push(&d);
}

/*
  ┌───────────────────────────────┐
  │   MULMOD                      │
  └───────────────────────────────┘
 */

// EVM MULMOD operation
// @param vm_state: VM state
void _mulmod(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t c = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t d = init_uint256(0);
  uint256_t e;

  // (a * b) % c
  mul_uint256(&a, &a, &b);
  equal_uint256(&c, &d) ? 0 : divmod_uint256(&e, &d, &a, &c);
  stack_push(&d);
}

/*
  ┌───────────────────────────────┐
  │   LT                          │
  └───────────────────────────────┘
 */

// EVM LT operation
// @param vm_state: VM state
void _lt(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  lt_uint256(&a, &b) ? change_all_uint256(&a, 0, 0, 0, 1)
                     : change_all_uint256(&a, 0, 0, 0, 0);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   GT                          │
  └───────────────────────────────┘
 */

// EVM GT operation
// @param vm_state: VM state
void _gt(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  gt_uint256(&a, &b) ? change_all_uint256(&a, 0, 0, 0, 1)
                     : change_all_uint256(&a, 0, 0, 0, 0);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   EQ                          │
  └───────────────────────────────┘
 */

// EVM EQ operation
// @param vm_state: VM state
void _eq(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  equal_uint256(&a, &b) ? change_all_uint256(&a, 0, 0, 0, 1)
                        : change_all_uint256(&a, 0, 0, 0, 0);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   ISZERO                      │
  └───────────────────────────────┘
 */

// EVM ISZERO operation
// @param vm_state: VM state
void _iszero(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = init_uint256(0);

  equal_uint256(&a, &b) ? change_all_uint256(&b, 0, 0, 0, 1)
                        : change_all_uint256(&b, 0, 0, 0, 0);
  stack_push(&b);
}

/*
  ┌───────────────────────────────┐
  │   AND                         │
  └───────────────────────────────┘
 */

// EVM AND operation
// @param vm_state: VM state
void _and(vm_state_t *vm) {
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
// @param vm_state: VM state
void _or(vm_state_t *vm) {
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
// @param vm_state: VM state
void _xor(vm_state_t *vm) {
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
// @param vm_state: VM state
void _not(vm_state_t *vm) {
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
// @param vm_state: VM state
void _shl(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  lshift_uint256(&a, &b, E11(a));
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   SHR                         │
  └───────────────────────────────┘
 */

// EVM SHR operation
// @param vm_state: VM state
void _shr(vm_state_t *vm) {
  uint256_t a = stack_peak(stack_length() - 1);
  stack_pop();

  uint256_t b = stack_peak(stack_length() - 1);
  stack_pop();

  rshift_uint256(&a, &b, E11(a));
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   Gas Limit                   │
  └───────────────────────────────┘
 */

// EVM GASLIMIT operation
// @param vm_state: VM state
void _gaslimit(vm_state_t *vm) {
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
// @param vm: VM state
void _mload(uint256_t memory[], vm_state_t *vm) {}

/*
  ┌───────────────────────────────┐
  │   MSTORE                      │
  └───────────────────────────────┘
 */

// EVM MSTORE operation
// @param memory[]: the memory
// @param vm: VM state
void _mstore(uint256_t memory[], vm_state_t *vm) {
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
  uint256_t max_mem_len = init_all_uint256(0, 0, 0, MAX_MEMORY_LEN - 1);

  uint64_t index = E11(a) / 32;
  uint64_t index2 = index + 1;
  uint64_t ending_index = index;
  uint64_t offset = ((E11(a)) % 32) * 8;

  // memory bounds check
  gt_uint256(&a, &max_mem_len) ? custom_error(VM_MEMORY_SIZE_EXCEEDED) : 0;

  //              gas cost stuff              //

  // check if memory has been expanded before
  !vm->mem_expanded ? vm->mem_expanded = true && vm->gas - 1 : 0;

  switch (offset) {
  case 0:
    break;
  default:
    ending_index += 1;
    if (ending_index - vm->mem_end > 0) {
      vm->gas -= ending_index * 3;
      vm->mem_end = ending_index;
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
// @param vm: VM state
void _mstore8(uint256_t memory[], vm_state_t *vm) {
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
  uint256_t max_mem_len = init_all_uint256(0, 0, 0, 0x0F4239);

  uint64_t begin_index = E11(a) / 32;
  uint64_t end_index = begin_index + 1;
  uint64_t ending_index = begin_index;
  uint64_t offset = ((E11(a)) % 32) * 8;

  // memory bounds check
  gt_uint256(&a, &max_mem_len) ? custom_error(VM_MEMORY_SIZE_EXCEEDED) : 0;

  //              gas cost stuff              //

  // check if memory has been expanded before
  !vm->mem_expanded ? vm->mem_expanded = true && vm->gas - 1 : 0;

  switch (offset) {
  case 0:
    break;
  default:
    ending_index += 1;
    if (ending_index - vm->mem_end > 0) {
      vm->gas -= ending_index * 3;
      vm->mem_end = ending_index;
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
// @param vm: VM state
void _pc(vm_state_t *vm) {
  uint256_t a = init_all_uint256(0, 0, 0, vm->pc - 1);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   MSIZE                       │
  └───────────────────────────────┘
 */

// EVM MSIZE operation
// @param vm: VM state
void _msize(vm_state_t *vm) {
  uint256_t a = init_all_uint256(0, 0, 0, vm->mem_end * 8);
  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   GAS                         │
  └───────────────────────────────┘
 */

// EVM GAS operation
// @param vm: VM state
void _gas(vm_state_t *vm) {
  uint256_t a = init_all_uint256(0, 0, 0, vm->gas);
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
void _push(uint256_t program[], vm_state_t *vm) {

  // masks for getting data from program[] to push
  uint256_t mask256_1 = init_uint256(0xFFFFFFFFFFFFFFFF);
  uint256_t mask256_2 = init_uint256(0xFFFFFFFFFFFFFFFF);

  // place to store the data taken from program[] to push
  uint256_t data1 = init_uint256(0);
  uint256_t data2 = init_uint256(0);

  // calculate bytes to push from program (PUSH1 starts at 96)
  int num_bytes2push = vm->opcode - 95;

  // keep track of where to start pushing bytes from in program[]
  int prev_pc = vm->pc;

  // increment program counter to next instruction
  vm->pc = prev_pc + num_bytes2push;

  // the index of the first byte to push
  int starting_index = prev_pc / 32;

  // the index of the last bytes to push
  int ending_index = (vm->pc - 1) / 32;

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
    rshift_uint256(&data1, &data1, (31 - ((vm->pc - 1) % 32)) * 8);

    // else get data from two uint256_ts in program[]
  } else {

    // first index //
    rshift_uint256(&mask256_1, &mask256_1, (prev_pc % 32) * 8);

    and_uint256(&data1, &program[starting_index], &mask256_1);

    lshift_uint256(&data1, &data1, 256 - (32 - (vm->pc % 32)) * 8);

    // second index //
    lshift_uint256(&mask256_2, &mask256_2, (32 - (vm->pc % 32)) * 8);

    and_uint256(&data2, &program[ending_index], &mask256_2);

    rshift_uint256(&data2, &data2, (31 - ((vm->pc - 1) % 32)) * 8);

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
void _dup(vm_state_t *vm) {
  // dup opcodes start @ 128 (0x80)
  int dup_index = vm->opcode - 127;

  uint256_t a = stack_peak(stack_length() - dup_index);

  stack_push(&a);
}

/*
  ┌───────────────────────────────┐
  │   SWAP                        │
  └───────────────────────────────┘
 */

// EVM SWAP operation
// @param vm: VM state
void _swap(vm_state_t *vm) {
  // swap opcodes start @ 144 (0x90)
  int swap_index = vm->opcode - 142;
  stack_swap(stack_length() - swap_index);
}

/*
  ┌───────────────────────────────┐
  │   MISC                        │
  └───────────────────────────────┘
 */

// comsume gas
// @param vm: VM state
void consume_gas(vm_state_t *vm) {}

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

void _vm(uint256_t program[], uint256_t memory[], vm_state_t *vm) {

  // for storing data to push onto stack
  uint256_t push_data;

  // while loop to run program //
  while (vm->pc < vm->max_pc) {
    get_opcode(program, vm);
    vm->pc += 1;

    switch (vm->opcode) {
    case 0x00: // STOP
      custom_error(STOP_INSTRUCTION);
      break;
    case 0x01: // ADD
      _add(vm);
      break;
    case 0x02: // MUL
      _mul(vm);
      break;
    case 0x03: // SUB
      _sub(vm);
      break;
    case 0x04: // DIV
      _div(vm);
      break;
    case 0x06: // MOD
      _mod(vm);
      break;
    case 0x08: // ADDMOD
      _addmod(vm);
      break;
    case 0x09: // MULMOD
      _mulmod(vm);
      break;
    case 0x10: // LT
      _lt(vm);
      break;
    case 0x11: // GT
      _gt(vm);
      break;
    case 0x14: // EQ
      _eq(vm);
      break;
    case 0x15: // ISZERO
      _iszero(vm);
      break;
    case 0x16: // AND
      _and(vm);
      break;
    case 0x17: // OR
      _or(vm);
      break;
    case 0x18: // XOR
      _xor(vm);
      break;
    case 0x19: // NOT
      _not(vm);
      break;
    case 0x1B: // SHL
      _shl(vm);
      break;
    case 0x1C: // SHR
      _shr(vm);
      break;
    case 0x45: // GASLIMIT
      _gaslimit(vm);
      break;
    case 0x50: // POP
      stack_pop(vm);
    case 0x51: // MLOAD
      _mload(memory, vm);
      break;
    case 0x52: // MSTORE
      _mstore(memory, vm);
      break;
    case 0x53: // MSTORE8
      _mstore8(memory, vm);
      break;
    case 0x59: // MSIZE
      _msize(vm);
      break;
    case 0x58: // PC
      _pc(vm);
      break;
    case 0x5A: // GAS
      _gas(vm);
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
      _push(program, vm);
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
      _dup(vm);
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
      _swap(vm);
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

void run_vm(uint256_t program[]) {
  vm_state_t vm;
  vm.pc = 0;
  vm.max_pc = 10;
  vm.gas = GAS - 21000;
  vm.mem_end = 0;

  static uint256_t memory[MAX_MEMORY_LEN];

  _vm(program, memory, &vm);
}