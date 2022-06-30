#include "h_stack.c"
#include "h_stack.h"
#include "uint256.h"
#include "processor.c"
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void) {
  static uint256_t program[MAX_BYTECODE_LEN];
  bool DEBUG = true;

  vm(program, &DEBUG);
  return 0;
}