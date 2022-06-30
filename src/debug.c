#include "debug.h"
#include "h_stack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void custom_error(char err_msg[]) {
  printf("%s\n", err_msg);
  exit(1);
}

// prints a buffer up to given length
// @param buffer: the buffer to print
// @param buff_name[]: name of buffer to print
// @param length: index to stop printing at
void print_buffer(uint256_t buffer[], char buff_name[], int length) {
  int i = 0;
  length = length - 1;

  printf("┌───────────────────────────────────────────────────────────────"
         "────────────┐\n│ %s                                                  "
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

// printfs the entire stack
// @param stack: the stack to print
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

// printf a specific stack index
// @param stack: which to stack to point to
// @param index: index to print from stack
void stack_peak_print(List *stack, int index) {
  assert(stack != NULL);

  uint256_t val = stack_peak(stack, index);

  print_hex_uint256(&val);
}
