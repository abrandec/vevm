#include "../../src/common/io/io.h"
#include "../../src/common/math/bigint/bigint.h"
#include "../../src/core/config.h"
#include "../../src/core/stack/stack.h"
#include "../../src/core/vm/vm.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// buffer for reading bytecode (accounts for space between uint64_t's)
#define CHAR_BUFF_LEN 49248

// write to program buffer
void write2_prog_buff(uint256_t program[]) {
  char char_bytecode[CHAR_BUFF_LEN];
  char *pEnd;

  // read_bytecode(program, char_bytecode);

  int i;

  int digits64;

  // writing this buffer sucks so I just do this for now
  program[0] =
      init_all_uint256(0x606960005300FFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);

  program[1] =
      init_all_uint256(0xFF60020200FFFFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);

  program[2] =
      init_all_uint256(0xFFFE0200FFFFFFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);
}

int main(int argc, char *argv[]) {
  static uint256_t program[MAX_BYTECODE_LEN];
  bool debug_mode = false;

  int i;

  for (i = 0; i < argc; ++i) {

    // debug flag
    if (strcmp(argv[i], "-DEBUG") == 0 || strcmp(argv[i], "-debug") == 0 ||
        strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "-d") == 0) {
      debug_mode = true;
      // help flag

    } else

        if (strncmp(argv[i], "-HELP", 1) == 0 ||
            strncmp(argv[i], "-help", 1) == 0 ||
            strncmp(argv[i], "-H", 1) == 0 || strncmp(argv[i], "-h", 1) == 0) {

      // Title + logo
      printf("Vision EVM Options      "
             "\033[93m▓▓\033[94m▓▓\033[92m▓▓\033[35m▓▓\033[91m▓▓\033[00m\n");
      printf("──────────────────────────────────\n");
      // Options
      printf("-debug, -d : debug mode\n"
             "-help,  -h : print this help msg\n");

      exit(0);
    }
  }

  char *filename = "output_hex/output.hex";

  print_file(filename);

  clear_buffer(program, MAX_BYTECODE_LEN);

  write2_prog_buff(program);

  _vm(program, debug_mode);

  return 0;
}