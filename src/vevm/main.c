#include "../../src/common/assets/assets.h"
#include "../../src/common/cmd/argparse/argparse.h"
#include "../../src/common/cmd/cmd.h"
#include "../../src/common/io/io.h"
#include "../../src/common/math/bigint/bigint.h"
#include "../../src/common/utils/hex_utils/hex_utils.h"
#include "../../src/core/config.h"
#include "../../src/core/vm/vm.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

// To get the compiler to not complain about formating for our custom integer
// types
#pragma GCC diagnostic ignored "-Wformat"

// argparse
static const char *const usages[] = {
    vevm_logo,
    NULL,
};

// argparse
#define PERM_READ (1 << 0)
#define PERM_WRITE (1 << 1)
#define PERM_EXEC (1 << 2)

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

// main program
int main(int argc, const char *argv[]) {
  static uint256_t program[MAX_BYTECODE_LEN];

  // to hold input and file chars from argparse
  char *file = NULL;
  char *input = NULL;

  // param variables
  int debug = 0;
  int perms = 0;

  struct argparse_option options[] = {
      OPT_GROUP("Commands"),
      OPT_HELP(),
      OPT_STRING('i', "input", &input, "input bytecode"),
      OPT_STRING('f', "file", &file, "file with bytecode", NULL, 0, 0),
      OPT_BOOLEAN('d', "debug", &debug, "print EVM debug", NULL, 0, 0),
      OPT_END(),
  };

  // parse arguments
  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argc = argparse_parse(&argparse, argc, argv);

  //┌────────────────────────────────────────────────┐
  //│                                                │
  //│   INPUT BYTECODE                               │
  //│                                                │
  //└────────────────────────────────────────────────┘

  // ┌───────────────────┐
  // │   INPUT BYTECODE  │
  // └───────────────────┘
  if (input != NULL && debug == 0) {
    write2_prog_buff(program, input, strlen(input));
    _vm(program, false);
  }

  // ┌───────────────────────────┐
  // │   INPUT BYTECODE + DEBUG  │
  // └───────────────────────────┘
  if (input != NULL && debug != 0) {
    write2_prog_buff(program, input, strlen(input));
    _vm(program, true);
  }

  //┌────────────────────────────────────────────────┐
  //│                                                │
  //│   READ FILE WITH BYTECODE                      │
  //│                                                │
  //└────────────────────────────────────────────────┘

  // ┌───────────────────────────┐
  // │   READ FILE WITH BYTCODE  │
  // └───────────────────────────┘
  if (file != NULL && debug == 0) {
    load_bytecode_file(program, file);
    _vm(program, false);
  }

  // ┌───────────────────────────────────┐
  // │   READ FILE WITH BYTCODE + DEBUG  │
  // └───────────────────────────────────┘
  if (file != NULL && debug != 0) {
    load_bytecode_file(program, file);
    _vm(program, true);
  }

  // ┌───────────────────────────────────┐
  // │   IF NO COMMAND EXISTS            │
  // └───────────────────────────────────┘
  if (argc != 0 || input == NULL && file == NULL) {
    argparse_usage(&argparse);
  }

  return 0;
}