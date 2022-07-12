#include "vm_t.h"

#include "../../../src/common/cmd/cmd.h"
#include "../../../src/common/io/io.h"
#include "../../../src/common/math/bigint/bigint.h"
#include "../../../src/common/utils/hex_utils/hex_utils.h"
#include "../../../src/core/config.h"
#include "../../../src/core/stack/stack.h"
#include "../../../src/core/vm/vm.h"
#include "../../test_utils/assert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   LOAD PROGRAM FROM FILE                                                   │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

// write to program buffer
// @param program[]: program buffer to write to
// @param bytecode: bytecode char to read from
// @param bytecode_size: size of bytecode
void write2_prog_buff(uint256_t program[], char *bytecode, long bytecode_size) {
  int elements = (bytecode_size / 16) + 1;

  int i = 0;
  for (; i < elements; ++i) {
    change_uint256(&program[i / 4], i % 4, hex_char2uint(bytecode, 16 * i, 16));
  }
}

// load bytecode from file
// @param program[]: program buffer to write to
// @param file: size of file to read from
void load_bytecode_file(uint256_t program[], char *file) {
  FILE *fd;

  long file_size;

  file = read_file_fmmap(fd, file, &file_size);

  file == NULL ? exit(1) : write2_prog_buff(program, file, file_size);

  safe_munmap(file, file_size);
}

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   TESTS                                                                    │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

bool assert_opcodes(uint256_t program[]) {
  char *file = "test_data/vm_data/opcodes";
  load_bytecode_file(program, file);

  int pc = 0;
  uint64_t opcode = 0;
  uint64_t gas = GAS - 21000;

  // EVM memory
  static uint256_t memory[MAX_MEMORY_LEN];

  // for keeping track memory expansion costs
  uint64_t mem_end = 0;

  // for checking if the first index in memory is being used (gas calc)
  bool mem_expanded = false;

  bool equality = true;

  int i = 0;
  for (; i < 255; ++i) {
    stack_reset();
    // set program counter to i
    pc = i;
    _vm(program, memory, &pc, i + 1, &opcode, &gas, &mem_expanded, &mem_end);
    equality = assert_eq(opcode, i);
  }

  return assert_msg("opcodes", equality);
}

bool assert_push(uint256_t program[]) {
  char *file = "test_data/vm_data/push_op";
  load_bytecode_file(program, file);
  stack_reset();
  int pc = 0;
  uint64_t opcode = 0;
  uint64_t gas = GAS - 21000;

  // EVM memory
  static uint256_t memory[MAX_MEMORY_LEN];

  // for keeping track memory expansion costs
  uint64_t mem_end = 0;

  // for checking if the first index in memory is being used (gas calc)
  bool mem_expanded = false;

  bool equality = true;

  int i = 0;

  _vm(program, memory, &pc, 528, &opcode, &gas, &mem_expanded, &mem_end);

  // it does the job I guess
  uint256_t peak32 = stack_peak(stack_length() - 1);
  uint256_t peak31 = stack_peak(stack_length() - 2);
  uint256_t peak30 = stack_peak(stack_length() - 3);
  uint256_t peak29 = stack_peak(stack_length() - 4);
  uint256_t peak28 = stack_peak(stack_length() - 5);
  uint256_t peak27 = stack_peak(stack_length() - 6);
  uint256_t peak26 = stack_peak(stack_length() - 7);
  uint256_t peak25 = stack_peak(stack_length() - 8);
  uint256_t peak24 = stack_peak(stack_length() - 9);
  uint256_t peak23 = stack_peak(stack_length() - 10);
  uint256_t peak22 = stack_peak(stack_length() - 11);
  uint256_t peak21 = stack_peak(stack_length() - 12);
  uint256_t peak20 = stack_peak(stack_length() - 13);
  uint256_t peak19 = stack_peak(stack_length() - 14);
  uint256_t peak18 = stack_peak(stack_length() - 15);
  uint256_t peak17 = stack_peak(stack_length() - 16);
  uint256_t peak16 = stack_peak(stack_length() - 17);
  uint256_t peak15 = stack_peak(stack_length() - 18);
  uint256_t peak14 = stack_peak(stack_length() - 19);
  uint256_t peak13 = stack_peak(stack_length() - 20);
  uint256_t peak12 = stack_peak(stack_length() - 21);
  uint256_t peak11 = stack_peak(stack_length() - 22);
  uint256_t peak10 = stack_peak(stack_length() - 23);
  uint256_t peak9 = stack_peak(stack_length() - 24);
  uint256_t peak8 = stack_peak(stack_length() - 25);
  uint256_t peak7 = stack_peak(stack_length() - 26);
  uint256_t peak6 = stack_peak(stack_length() - 27);
  uint256_t peak5 = stack_peak(stack_length() - 28);
  uint256_t peak4 = stack_peak(stack_length() - 29);
  uint256_t peak3 = stack_peak(stack_length() - 30);
  uint256_t peak2 = stack_peak(stack_length() - 31);
  uint256_t peak1 = stack_peak(stack_length() - 32);

  equality = assert_eq_uint256(peak1, init_all_uint256(0, 0, 0, 0x01));
  equality = assert_eq_uint256(peak2, init_all_uint256(0, 0, 0, 0x0220));
  equality = assert_eq_uint256(peak3, init_all_uint256(0, 0, 0, 0x033300));
  equality = assert_eq_uint256(peak4, init_all_uint256(0, 0, 0, 0x04444400));
  equality = assert_eq_uint256(peak5, init_all_uint256(0, 0, 0, 0x0555555000));
  equality =
      assert_eq_uint256(peak6, init_all_uint256(0, 0, 0, 0x066666660000));
  equality =
      assert_eq_uint256(peak7, init_all_uint256(0, 0, 0, 0x07777777700000));
  equality =
      assert_eq_uint256(peak8, init_all_uint256(0, 0, 0, 0x0888888880000000));
  equality = assert_eq_uint256(
      peak9, init_all_uint256(0, 0, 0x09, 0x9999999000000009));
  equality = assert_eq_uint256(
      peak10, init_all_uint256(0, 0, 0xa0aa, 0xaaaaaa000000000a));
  equality = assert_eq_uint256(
      peak11, init_all_uint256(0, 0, 0x04bbbb, 0xbbbb0000000000b0));
  equality = assert_eq_uint256(
      peak12, init_all_uint256(0, 0, 0x0c5ccccc, 0xcc00000000000c00));
  equality = assert_eq_uint256(
      peak13, init_all_uint256(0, 0, 0x0dd4dddddd, 0x000000000000d000));
  equality = assert_eq_uint256(
      peak14, init_all_uint256(0, 0, 0x0eee6eeeee00, 0x000000000000e000));
  equality = assert_eq_uint256(
      peak15, init_all_uint256(0, 0, 0x0ffff7ffff0000, 0x00000000000f0000));
  equality = assert_eq_uint256(
      peak16, init_all_uint256(0, 0, 0x100000300000ee00, 0x0000000000000001));
  equality =
      assert_eq_uint256(peak17, init_all_uint256(0, 0x10, 0x0000050000ffffff,
                                                 0xffffffffffffff01));
  equality =
      assert_eq_uint256(peak18, init_all_uint256(0, 0x1000, 0x00005000000000fe,
                                                 0xec00000000000015));
  equality = assert_eq_uint256(
      peak19,
      init_all_uint256(0, 0x10e000, 0x000500000000ffff, 0xfffffeffffffff01));
  equality = assert_eq_uint256(
      peak20,
      init_all_uint256(0, 0x10000000, 0x0040000000ffdecb, 0xaeeeeeeeeeffff01));
  equality = assert_eq_uint256(peak21, init_all_uint256(0, 0x1000000000,
                                                        0x06000000fffffdaa,
                                                        0xcbbbbeeeefffff0a));
  equality = assert_eq_uint256(peak22, init_all_uint256(0, 0x100000000000,
                                                        0x600000ffffffffff,
                                                        0xacbbccccccffff01));
  equality = assert_eq_uint256(peak23, init_all_uint256(0, 0x10000000000007,
                                                        0x0000ffffffffaab4,
                                                        0x383485ffffffff01));
  equality = assert_eq_uint256(peak24, init_all_uint256(0, 0x1000000000000080,
                                                        0x00fffffffffff345,
                                                        0x43ffffffffffff01));
  equality = assert_eq_uint256(
      peak25, init_all_uint256(0x10, 0x0000000000000900, 0xfff4343435443544,
                               0xffffffffffffff01));
  equality = assert_eq_uint256(
      peak26, init_all_uint256(0x1000, 0x00000000000090ff, 0xffffffffffffffff,
                               0xfffffffff5434f01));
  equality = assert_eq_uint256(
      peak27, init_all_uint256(0x100000, 0x000000000005ffff, 0xfffffffffff34534,
                               0xfffffff111ffff01));
  equality = assert_eq_uint256(
      peak28, init_all_uint256(0x10000000, 0x000000000f5fffff,
                               0xffffffffffffffff, 0xff34344221211221));
  equality = assert_eq_uint256(
      peak29, init_all_uint256(0x1000000000, 0x0000000ff5ffffff,
                               0xffffffffffff9438, 0x9349348932221121));
  equality = assert_eq_uint256(
      peak30, init_all_uint256(0x100000000000, 0x00000fff5fffffff,
                               0xffffff9876543234, 0x5678987676544322));
  equality = assert_eq_uint256(
      peak31, init_all_uint256(0x10000000000000, 0x000ffff5ffffffff,
                               0xffffffffff548945, 0x8554934338923233));
  equality = assert_eq_uint256(
      peak32, init_all_uint256(0x4345785487453232, 0x3982385923934843,
                               0x3489343493484334, 0x9438349439834943));

  return assert_msg("push", equality);
}

bool assert_mem_bounds(uint256_t program[]) {

  char *file = "test_data/vm_data/mem_bounds";
  load_bytecode_file(program, file);

  stack_reset();

  int pc = 0;
  uint64_t opcode = 0;
  uint64_t gas = GAS - 21000;

  // EVM memory
  static uint256_t memory[MAX_MEMORY_LEN];

  // for keeping track memory expansion costs
  uint64_t mem_end = 0;

  // for checking if the first index in memory is being used (gas calc)
  bool mem_expanded = false;

  _vm(program, memory, &pc, 10, &opcode, &gas, &mem_expanded, &mem_end);

  mem_expanded ? 0 : assert_msg("memory not expanded", false);

  return assert_msg("mem_bounds", mem_expanded);
}

// set sample program for testing
// @param program: the program to set
void set_sample_prog(uint256_t program[]) {}

// set memory for testing
// @param memory: the memory to set
void set_memory(uint256_t memory[]) {}

// vm tests
bool vm_tests(void) {
  static uint256_t program[MAX_BYTECODE_LEN];

  // Run tests
  enum { test_len = 3 };
  bool test_results[test_len] = {assert_opcodes(program), assert_push(program),
                                 assert_mem_bounds(program)};
  return assert_bool_array_msg("VM results", test_results, test_len);
}
