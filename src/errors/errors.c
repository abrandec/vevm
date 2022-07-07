#include "errors.h"

#include "../common/cmd/cmd.h"
#include "../core/stack/stack.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void custom_error(uint8_t err_code) {

  // ┌──────────────────────────────────┐
  // │           ERROR  TABLE           │
  // ├──────────────────────────────────┤
  // │ Stack errors         0xA0 - 0xAF │
  // ├──────┬───────────────────────────┤
  // │ 0xA0 │ Stack overflow            │
  // │ 0xA1 │ Stack underflow           │
  // │ 0xA2 │ Stack index out of bounds │
  // │ 0xA3 │ Stack NULL                │
  // ├──────┴───────────────────────────┤
  // │ VM errors            0xB0 - 0xDF │
  // ├──────┬───────────────────────────┤
  // │ 0xB0 │ Invalid opcode            │
  // │ 0xB1 │ Invalid argument          │
  // │ 0xB2 │ Memory size exceeded      │
  // ├──────┴───────────────────────────┤
  // │ IO errors            0xE0 - 0xEF │
  // ├──────┬───────────────────────────┤
  // │ 0xE0 │ File not found            │
  // │ 0xE1 │ File not opened           │
  // │ 0xE2 │ File not closed           │
  // └──────┴───────────────────────────┘
#ifdef DEBUG
  printf(YELLOW "Error code: %X\n" RESET, err_code);
  switch (err_code) {
  // Stack //
  case 0xA0:
    printf("└ Stack overflow\n");
    break;
  case 0xA1:
    printf("└ Stack underflow\n");
    break;
  case 0xA2:
    printf("└ Stack index out of bounds\n");
    break;
  case 0xA3:
    printf("└ Stack NULL\n");
    break;
  // VM //
  case 0xB0:
    printf("└ Invalid opcode\n");
    break;
  case 0xB1:
    printf("└ Invalid argument\n");
    break;
  case 0xB2:
    printf("└ Memory size exceeded\n");
    break;
  // IO //
  case 0xE0:
    printf("└ File not found\n");
    break;
  case 0xE1:
    printf("└ File not opened\n");
    break;
  case 0xE2:
    printf("└ File not closed\n");
    break;
  }
#endif
#ifdef TEST

#endif
}