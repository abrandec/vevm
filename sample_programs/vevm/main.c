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

// buffer for reading bytecode (accounts for space between uint64_t's)
#define CHAR_BUFF_LEN 49248

static const char *const usages[] = {
    vevm_logo,
    NULL,
};

#define PERM_READ (1 << 0)
#define PERM_WRITE (1 << 1)
#define PERM_EXEC (1 << 2)

// write to program buffer
void write2_prog_buff(uint256_t program[]) {
  char char_bytecode[CHAR_BUFF_LEN];

  int digits64;

  // writing this buffer sucks so I just do this for now
  program[0] =
      init_all_uint256(0x6008600A600A0800, UINT64_MAX, UINT64_MAX, UINT64_MAX);

  program[1] =
      init_all_uint256(0xFF60020200FFFFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);

  program[2] =
      init_all_uint256(0xFFFE0200FFFFFFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);
}

int main(int argc, const char *argv[]) {
  static uint256_t program[MAX_BYTECODE_LEN];

  FILE *fd;
  bool debug_mode = false;

  char *path = NULL;
  char *file;
  char *input = "idk";

  // param variables
  int debug = 0;
  int perms = 0;

  struct argparse_option options[] = {
      OPT_GROUP("Commands"),
      OPT_HELP(),
      OPT_STRING('i', "input", &input, "input bytecode"),
      OPT_STRING('p', "path", &path, "path to hex file with bytecode", NULL, 0,
                 0),
      OPT_BOOLEAN('d', "debug", &debug, "print EVM debug", NULL, 0, 0),
      OPT_END(),
  };

  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argc = argparse_parse(&argparse, argc, argv);
  clear_buffer(program, MAX_BYTECODE_LEN);

  char *pEnd;

  char *str;

  // read_bytecode(program, char_bytecode);

  // Input bytecode //

  // Input bytecode & debug = false
  if (input != NULL && debug == 0) {
    E00(program[0]) = strtoull(input, &pEnd, 16);
    print_hex_uint256(&program[0], true);
    printf("%s\n", input);
  }

  // Input bytecode & debug mode = true
  if (input != NULL && debug != 0) {
    printf("%d\n", debug);
    printf("%s\n", input);
  }

  // Read bytecode from file //

  // Read bytecode from file & debug = false
  if (path != NULL && debug == 0) {

    //create_file(fd, path, input);
    // file = read_file_fmmap(fd, input);
    create_folder(path);
    // printf("%s\n", file);
    // print_file(fd, path);
    print_hex_uint256(&program[0], true);

    munmap(file, sizeof(file));
  }

  // Read bytecode from file & debug mode
  if (path != NULL && debug != 0) {
    printf("%s\n", path);
    print_file(fd, path);
    // load file && write to buffer
    // run vm
    debug_mode = true;
    //_vm(program, debug_mode);
  }

  // If no input command exists //
  if (argc != 0) {
    printf("Refer to -help for usage\n");
  }

  return 0;
}