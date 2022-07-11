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

// write to program buffer
// @param program[]: program buffer to write to
// @param bytecode: bytecode char to read from
// @param bytecode_size: size of bytecode
void write2_prog_buff(uint256_t program[], char *bytecode, long bytecode_size) {
  int elements = (bytecode_size / 16) + 1;

  int i = 0;
  // side effect of adding unwanted bits to the end of the bytecode
  // not really a problem since adding STOP to the end of the bytecode mitigates
  // this
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

  return assert_msg("mem_bounds", true);
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
  enum { test_len = 1 };
  bool test_results[test_len] = {assert_mem_bounds(program)};
  return assert_bool_array_msg("VM results", test_results, test_len);
}
