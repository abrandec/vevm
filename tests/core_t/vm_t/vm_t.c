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

const static char incorrect_value[20] = "incorrect value\0";

/* These test have a lot of room for improvement */

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

// for quick testing on arithmetic
// @param program[]: program buffer
// @param memory[]: memory buffer
void hot_load_vm(uint256_t program[], uint256_t memory[]) {
  stack_reset();
  vm_state_t vm;
  vm.pc = 0;
  vm.max_pc = 10;
  vm.opcode = 0;
  vm.gas = 0;
  vm.mem_end = 0;

  _vm(program, memory, &vm);
}

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   TESTS                                                                    │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

bool assert_get_opcodes(uint256_t program[]) {
  char *file = "test_data/vm_data/opcodes";
  load_bytecode_file(program, file);
vm_state_t vm;
  vm.pc = 0;
  vm.opcode = 0;
  vm.gas = 0;
  vm.mem_end = 0;

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
    vm.pc = i;
    vm.max_pc = i + 1;
    _vm(program, memory, &vm);
    equality = assert_eq(vm.opcode, i);
  }

  return equality;
}

// test every push opcode and confirm values on the stack are correct
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_push(uint256_t program[], uint256_t memory[]) {
  char *file = "test_data/vm_data/push_op";
  load_bytecode_file(program, file);
  stack_reset();
  vm_state_t vm;
  vm.pc = 0;
  vm.max_pc = 528;
  vm.opcode = 0;
  vm.gas = 0;
  vm.mem_end = 0;

  bool equality = true;


  _vm(program, memory, &vm);

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

  return equality;
}

// assert VM memory bounds cannot be exceeded
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_mem_bounds(uint256_t program[], uint256_t memory[]) {

  char *file = "test_data/vm_data/mem_bounds";
  load_bytecode_file(program, file);

  stack_reset();
  vm_state_t vm;
  vm.pc = 0;
  vm.max_pc = 10;
  vm.opcode = 0;
  vm.gas = 0;
  vm.mem_end = 0;

  _vm(program, memory, &vm);

  vm.mem_expanded ? 0 : assert_msg("memory not expanded", false);

  return assert_msg("mem_bounds", vm.mem_expanded);
}

// assert addition on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_add(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "add error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600B0100, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x15);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;
  return assert_msg(__func__, status);
}

// assert multiplication on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_mul(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "mul error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6055600560050200, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x19);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert subtraction on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_sub(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "sub error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600C0300, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x02);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert division on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_div(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "div error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert sdiv on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_sdiv(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "sdiv error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6055600360220500, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x0B);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert mod on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_mod(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "mod error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x60336007605806, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x04);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert smod on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_smod(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "smod error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert addmod on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_addmod(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "addmod error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6008600760550800, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x04);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert mulmod on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_mulmod(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "mulmod error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6008600760550900, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x03);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert exp on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_exp(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "exp error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert lt on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_lt(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "lt error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6008605560071000, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 1);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert gt on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_gt(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "gt error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6008600760551100, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 1);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert slt on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_slt(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "slt error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert sgt on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_sgt(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "sgt error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);

  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert eq on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool _assert_eq(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "eq error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6008605560551400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 1);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert iszero on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_iszero(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "iszero error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6008605560001500, 0, 0, 0);

  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 1);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert and on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_and(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "and error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x601160FD60FE1600, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0xFC);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert or on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_or(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "or error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x601160FA60FC1700, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0xFE);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert xor on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_xor(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "xor error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x601160FA60FC1800, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x06);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert not on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_not(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "not error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x601160FA60FC1900, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                        0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFF03);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert iszero on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_byte(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "byte error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x601160FA60FC1A00, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert shl on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_shl(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "shl error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x601160FA60FC1B00, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0xA000000000000000, 0, 0, 0);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert shr on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_shr(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "shr error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x601160FF60011C00, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x7F);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert sha3 on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_sha3(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "sha3 error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);

  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert address on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_address(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "address error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert balance on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_balance(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "balance error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert origin on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_origin(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "balance error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert caller on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_caller(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "caller error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert callvalue on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_callvalue(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "caller error",
                          ASSERT_UINT256_T, false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
  assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert calldataload on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_calldataload(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "calldataload error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert calldatasize on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_calldatasize(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "calldatasize error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// assert calldatacopy on stack
// @param: program[]: program buffer
// @param: memory[]: memory buffer
bool assert_calldatacopy(uint256_t program[], uint256_t memory[]) {
  error_t assert_err;
  bool status = set_error(&assert_err, __func__, "calldatacopy error", ASSERT_UINT256_T,
                          false, false);

  program[0] = init_all_uint256(0x6011600A600F0400, 0, 0, 0);
  hot_load_vm(program, memory);

  uint256_t expected = init_all_uint256(0, 0, 0, 0x01);
  uint256_t actual = stack_peak(stack_length() - 1);
assert_err.expected = &expected;
  assert_err.actual = &actual;

  !assert_eq_uint256(expected, actual) ? status = assert_error(&assert_err) : 0;

  return assert_msg(__func__, status);
}

// vm tests
bool vm_tests(void) {
  static uint256_t program[MAX_BYTECODE_LEN];
  static uint256_t memory[MAX_MEMORY_LEN];

  // Run tests
  enum { test_len = 25 };

  bool test_results[test_len] = {
      assert_get_opcodes(program), assert_push(program, memory),
      assert_add(program, memory), assert_mul(program, memory),
      assert_sub(program, memory),
      // test more below
      assert_div(program, memory), assert_sdiv(program, memory),
      assert_mod(program, memory), assert_smod(program, memory),
      assert_addmod(program, memory), assert_mulmod(program, memory),
      assert_exp(program, memory), assert_lt(program, memory),
      assert_gt(program, memory), assert_slt(program, memory),
      assert_sgt(program, memory), _assert_eq(program, memory),
      assert_iszero(program, memory), assert_and(program, memory),
      assert_or(program, memory), assert_xor(program, memory),
      assert_not(program, memory), assert_byte(program, memory),
      assert_shl(program, memory), assert_shr(program, memory)};

  // assert_msg("mem bounds", assert_mem_bounds(program));
  return assert_bool_array_msg("VM TESTS", test_results, test_len);
}
