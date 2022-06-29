#include "h_stack.h"

#include "uint256.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create a node
// @return a ptr to the newly created node
Node *Node_create(void) {

  Node *node = malloc(sizeof(Node));

  assert(node != NULL);

  node->data = 0;

  node->next = NULL;

  return node;
}

// destroy a node
// @param node: the node to destroy
void Node_destroy(Node *node) {
  assert(node != NULL);
  free(node);
}

// create a new stack
// @returns a new stack
List *stack_create(void) {
  List *stack = malloc(sizeof(List));
  assert(stack != NULL);

  Node *node = Node_create();
  stack->first = node;

  return stack;
}

// destroy the stack
// @param stack: the stack to destroy
void stack_destroy(List *stack) {
  assert(stack != NULL);

  Node *node = stack->first;
  Node *next;
  while (node != NULL) {
    next = node->next;
    free(node);
    node = next;
  }

  free(stack);
}

// push a new e onto the top of the stack
// @param stack: the stack to push onto
// @param data: the data to push onto the stack
void stack_push(List *stack, uint256_t *val) {
  assert(stack != NULL);
  if (stack_length(stack) > MAX_STACK_DEPTH - 1) {
    printf("EVM - Stack Overflow\n");
    return;
  } else {

    Node *node = stack->first;
    while (node->next != NULL) {
      node = node->next;
    }

    uint256_t *value = malloc(sizeof(uint256_t));

    copy_uint256(value, val);

    node->data = value;

    node->next = Node_create();
  }
}

void stack_swap(List *stack, int index) {
  assert(stack != NULL);
  int i = stack_length(stack) - 1;
  int i2 = i;

  if (index < 0 || index > 15 || index > i) {
    printf("EVM - Stack element not accessable\n");
    exit(1);
  } else {
    uint256_t data2swap;

    Node *before = stack->first;
    Node *after = stack->first;

    while (i2 != 0) {
      after = after->next;
      --i2;
    }

    while (i != index) {
      before = before->next;
      --i;
    }

    // top of stack element
    copy_uint256(&data2swap, after->data);

    // element to swap with
    copy_uint256(after->data, before->data);

    copy_uint256(before->data, &data2swap);
  }
}

// peak at the stack at a certain index
// @param stack: stack to peek from
// @param index: index of the stack to peek
// @return stack item
uint256_t stack_peak(List *stack, int index) {
  uint256_t val = init_uint256(0);

  if (index < -1 || index > stack_length(stack) - 1) {
    printf("EVM - Stack address not accessable\n");

    return val;
  } else {
    Node *node = stack->first;
    int i;
    for (i = 0; i < index; ++i) {
      node = node->next;
    }

    copy_uint256(&val, node->data);
    return val;
  }
}

// pop the first element on the stack
// @param stack: stack
void stack_pop(List *stack) {
  assert(stack != NULL);
  int index = stack_length(stack) - 1;
  if (stack_length(stack) < 1) {
    printf("EVM - Stack Underflow\n");
    exit(1);
  } else {
    if (index == 0) {
      Node *node = stack->first;
      stack->first = stack->first->next;
      uint256_t *data = node->data;
      free(data);
      Node_destroy(node);
    } else {
      Node *before = stack->first;
      while (index > 0) {
        before = before->next;
        --index;
      }
      Node *node = before->next;
      before->next = before->next->next;
      uint256_t *data = node->data;
      free(data);
      Node_destroy(node);
    }
  }
}

// gets length of stack
// @param stack: stack to get length of
// @return length of stack
int stack_length(List *stack) {
  assert(stack != NULL);
  Node *node = stack->first;
  int length = 0;
  while (node->next != NULL) {
    ++length;
    node = node->next;
  }

  return length;
}

// printf functions

// printfs the entire stack
// @param stack: the stack to print
void stack_print(List *stack) {
  assert(stack != NULL);
  if (stack_length(stack) == 0) {
    printf("EVM - Stack is empty\n");
  } else {
    Node *node = stack->first;
    int i = stack_length(stack) - 1;
    printf("\n");
    printf("―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
           "――――――――\n");
    printf("Stack                                                              "
           "       |\n");
    printf("―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
           "――――――――\n");

    while (node->next != NULL) {
      printf("0x%03X: 0x", i);
      print_hex_uint256(node->data);
      printf(" |\n");
      node = node->next;
      --i;
      if (node->next != NULL) {
        printf("―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
               "――――――――――――\n");
      }
    }
    printf("―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
           "――――――――\n");
  }
}

// printf specific stack index
// @param stack: which to stack to point to
// @param index: address to print from stack
void stack_peak_print(List *stack, int index) {
  assert(stack != NULL);

  uint256_t val = stack_peak(stack, index);

  print_hex_uint256(&val);
}
