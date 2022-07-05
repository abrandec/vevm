#include "../../src//bigint/bigint.h"
#include "../../src/config.h"
#include "../../src/processor/processor.h"
#include "../../src/stack/stack.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// buffer for reading bytecode
#define CHAR_BUFF_LEN 49248

static const char *filename = "output_hex/output.hex";

// load bytecode from file & write to program buffer using write_bytecode
// @prog_buf: buffer to write to
void read_bytecode(uint256_t prog_buf[], char ch[]) {

  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Error: could not open file %s", filename);
    exit(1);
  }

  // read one character at a time and
  // display it to the output
  int i = 0;

  while ((ch[i] = fgetc(fp)) != EOF) {

    ++i;
    // only because im lazy and using strtoll that requires whitespace to read
    // more than 16 char at a time
    if (i % 16 == 0) {
      ch[i] = ' ';
      ++i;
    }
  }

  ++i;

  ch[i] = '\0';

  // func to convert string to hex

  // close the file
  fclose(fp);
}

// write to program buffer
void write2_prog_buff(uint256_t program[]) {
  char char_bytecode[CHAR_BUFF_LEN];
  char *pEnd;

  read_bytecode(program, char_bytecode);

  int i;

  int digits64;

  // writing this buffer sucks so I just do this for now
  program[0] =
      init_all_uint256(0x600019001800FFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);

  program[1] =
      init_all_uint256(0xFF60020200FFFFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);

  program[2] =
      init_all_uint256(0xFFFE0200FFFFFFFF, UINT64_MAX, UINT64_MAX, UINT64_MAX);
}

int main(int argc, char *argv[]) {
  static uint256_t program[MAX_BYTECODE_LEN];

  int i;

  for (i = 0; i < argc; ++i) {
#ifdef DEBUG
    // debug flag
    if (strcmp(argv[i], "-DEBUG") == 0 || strcmp(argv[i], "-debug") == 0 ||
        strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "-d") == 0) {

      // help flag

    } else
#endif
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

  clear_buffer(program, MAX_BYTECODE_LEN);

  // read_bytecode(program);

  write2_prog_buff(program);

  vm(program);
  return 0;
}