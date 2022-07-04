#include "../../src/bigint.h"
#include "../../src/config.h"
#include "../../src/processor.h"
#include "../../src/stack.h"

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
int read_bytecode(uint256_t prog_buf[], char ch[]) {

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

  return i - 4;
}

// write to program buffer
void write2_prog_buff(uint256_t program[]) {
  char char_bytecode[CHAR_BUFF_LEN];
  char *pEnd;

  // number of characters in file
  int size = read_bytecode(program, char_bytecode);

  int i;
  int index = size / 32;

  int uint256_index = index / 4;
  int digits64;
  uint64_t temp;
  uint256_t t = init_all_uint256(0x0100000000000000, 0x0000000000000000,
                                 0x0000000000000000, 0x0000000000000000);

  printf("index %d\n", index);

  // writing this buffer sucks so I just do this for now
  program[uint256_index] =
      init_all_uint256(0x60ff600052606460, 0x1F52606460305260,
                       0x69602F5200000000, 0x0000000000000000);
}

int main(int argc, char *argv[]) {
  static uint256_t program[MAX_BYTECODE_LEN];

  bool DEBUG = false;
  int i;

  for (i = 0; i < argc; ++i) {

    // debug flag
    if (strcmp(argv[i], "-DEBUG") == 0 || strcmp(argv[i], "-debug") == 0 ||
        strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "-d") == 0) {
      DEBUG = true;

      // help flag
    } else if (strncmp(argv[i], "-HELP", 1) == 0 ||
               strncmp(argv[i], "-help", 1) == 0 ||
               strncmp(argv[i], "-H", 1) == 0 ||
               strncmp(argv[i], "-h", 1) == 0) {

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

  program[0] = init_uint256(0);

  // read_bytecode(program);

  write2_prog_buff(program);

  vm(program, &DEBUG);
  return 0;
}