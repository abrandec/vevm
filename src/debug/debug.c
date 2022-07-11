#include "debug.h"

#include "../common/cmd/cmd.h"
#include "../common/math/bigint/bigint.h"
#include "../core/opcodes/op_names.h"
#include "../core/stack/stack.h"
#include "../common/assets/assets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Stack debugging */
void print_stack() {
  
  if (stack_length() <= 0) {
    printf(
        "\n┌───────────────────────────────────────────────────────────────"
        "────────────┐\n│ STACK                                                "
        "              "
        "       "
        "│\n├────────────────────────────────────────────────────────────────"
        "───────────┤\n│                                                       "
        "         "
        "           "
        "│\n└────────────────────────────────────────────────────────────────"
        "───────────┘\n");

  } else {
    
    int i = stack_length() - 1;

    printf(
        "\n┌───────────────────────────────────────────────────────────────"
        "────────────┐\n│ STACK                                             "
        "                 "
        "       "
        "│\n├────────────────────────────────────────────────────────────────"
        "───────────┤\n");

    while (1) {
      printf("│ 0x%03X: 0x", i);
      
      printf(" │\n");
      
      --i;

      if (1) {
        printf(
            "├────────────────────────────────────────────────────────────────"
            "───────────┤\n");
      }
    }
    printf("└────────────────────────────────────────────────────────────────"
           "───────────┘\n");
  }
}

void print_buffer(uint256_t buffer[], const char buff_name[], int length) {
  int i = 0;

  printf("┌───────────────────────────────────────────────────────────────"
         "────────────┐\n│ %s                                                 "
         "            "
         "      "
         "│\n├────────────────────────────────────────────────────────────────"
         "───────────┤\n",
         buff_name);

  for (; i < length; ++i) {
    printf("│ 0x%03X: 0x", i);
    print_hex_uint256(&buffer[i], false);
    printf(" │\n");
  }

  printf("└────────────────────────────────────────────────────────────────"
         "───────────┘\n");
}

void print_debug(uint256_t memory[], int *pc, uint64_t *gas,
                 uint64_t *opcode) {
  static const char prog_name[8] = "PROGRAM";
  static const char mem_name[8] = "MEMORY ";

  system("clear");
  printf("%s\n┌─────────────────────────────────┐\n│ OPCODE   %02lX          "
         "           │ %s\n│ PC       %06d                 │\n│ GAS      "
         "%06lu   │\n└─────────────────────────────────┘",
         vevm_logo, *opcode, OP_NAME[*opcode], *pc, *gas);
  print_stack();

  print_buffer(memory, mem_name, PRINT_LENGTH);

  sleep(1);
}
