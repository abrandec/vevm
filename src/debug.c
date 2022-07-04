#include "debug.h"
#include "bigint.h"
#include "stack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void custom_error(char err_msg[]) {
  printf("%s\n", err_msg);
  exit(1);
}

void print_buffer(uint256_t buffer[], char buff_name[], int length) {
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
