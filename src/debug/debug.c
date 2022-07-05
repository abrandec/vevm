#include "../common/math/bigint/bigint.h"
#include "../core/opcodes/op_names.h"
#include "../core/stack/stack.h"

#include "debug.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    print_hex_uint256(&buffer[i]);
    printf(" │\n");
  }

  printf("└────────────────────────────────────────────────────────────────"
         "───────────┘\n");
}

void print_debug(List *stack, uint256_t memory[], int *pc, uint64_t *gas,
                 uint64_t *opcode) {
  const char prog_name[8] = "PROGRAM";
  const char mem_name[8] = "MEMORY ";

  system("clear");

  printf("\033[93m▓▓\033[94m▓▓\033[92m▓▓\033[35m▓▓\033[91m▓▓\033["
         "00m\n┌─────────────────────────────────┐\n│ OPCODE   %02lX          "
         "           │ %s\n│ PC       %06d                 │\n│ GAS      "
         "%06lu   │\n└─────────────────────────────────┘",
         *opcode, OP_NAME[*opcode], *pc, *gas);
  stack_print(stack);

  print_buffer(memory, mem_name, PRINT_LENGTH);

  sleep(1);
}

/* Stack debugging */

void stack_print(List *stack) {
  assert(stack != NULL);
  if (stack_length(stack) == 0) {
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
    Node *node = stack->first;
    int i = stack_length(stack) - 1;

    printf(
        "\n┌───────────────────────────────────────────────────────────────"
        "────────────┐\n│ STACK                                             "
        "                 "
        "       "
        "│\n├────────────────────────────────────────────────────────────────"
        "───────────┤\n");

    while (node->next != NULL) {
      printf("│ 0x%03X: 0x", i);
      print_hex_uint256(node->data);
      printf(" │\n");
      node = node->next;
      --i;

      if (node->next != NULL) {
        printf(
            "├────────────────────────────────────────────────────────────────"
            "───────────┤\n");
      }
    }
    printf("└────────────────────────────────────────────────────────────────"
           "───────────┘\n");
  }
}

void stack_peak_print(List *stack, int index) {
  assert(stack != NULL);

  uint256_t val = stack_peak(stack, index);

  print_hex_uint256(&val);
}
