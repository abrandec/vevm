#include "vm_t.h"

#include "../../../src/common/math/bigint/bigint.h"
#include "../../../src/core/stack/stack.h"
#include "../../../src/core/vm/vm.h"
#include "../../test_utils/assert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool assert_opcode(uint256_t program[], int expected) {
  int pc = 0;
  uint64_t opcode = 0;

  //_vm(program, true, &pc, &opcode);

  return assert_eq_msg("Opcode check", opcode, false);
  return true;
}

bool assert_mem_bounds(uint256_t program[], int expected) {

  //_vm(program, true, &pc, &opcode);

  return assert_eq_msg("Mem bounds check", 0, 0);
}

// set sample program for testing
// @param program: the program to set
void set_sample_prog(uint256_t program[]) {
  

  // set bytecode //
  change_all_uint256(&program[0], 0x1110203040500700, 0x01, 0x02, 0x03);
  change_all_uint256(&program[1], 0x04, 0x05, 0x06, 0x07);
  change_all_uint256(&program[2], 0x09, 0x0A, 0x0B, 0x0C);
  change_all_uint256(&program[3], 0x0E, 0x0F, 0x10, 0x11);

  // set bytecode for last index
  change_all_uint256(&program[MAX_BYTECODE_LEN - 1], 0x13, 0x14, 0x15, 0x16);
}

// set memory for testing
// @param memory: the memory to set
void set_memory(uint256_t memory[]) {
  // set memory //
  change_all_uint256(&memory[0], 0x1110203040500700, 0x01, 0x02, 0x03);
  change_all_uint256(&memory[1], 0x04, 0x05, 0x06, 0x07);
  change_all_uint256(&memory[2], 0x09, 0x0A, 0x0B, 0x0C);
  change_all_uint256(&memory[3], 0x0E, 0x0F, 0x10, 0x11);

  // set memory for last index
  change_all_uint256(&memory[MAX_MEMORY_LEN - 1], 0x13, 0x14, 0x15, 0x16);
}

// vm tests
bool vm_tests(void) {
  static uint256_t program[MAX_BYTECODE_LEN];

  // Run tests
  enum { test_len = 2 };
  bool test_results[test_len] = {assert_opcode(program, 0), assert_mem_bounds(program, 0)};
  return assert_bool_array_msg("VM results", test_results, test_len);
}
