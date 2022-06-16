#include "h_stack.h"
#include "uint256.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_DEPTH 10

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

    node->data = val;
    node->next = Node_create();
  }
}

// peak an e from the stack
// @param stack: stack to peek from
// @param index: index of the e to peek
// @return pointer to the e on stack
uint256_t *stack_peak(List *stack, int index) {
  assert(stack != NULL);
  if (index < -1) {
    printf("EVM - Stack address does not exist\n");
    return NULL;
  } else {
    Node *node = stack->first;
    for (int i = 0; i < index; ++i) {
      node = node->next;
      --index;
    }
    return node->data;
  }
}

// pop 1 e from top of stack
// @param stack: stack to use
void stack_pop(List *stack) {
  assert(stack != NULL);
  int index = stack_length(stack) - 1;
  if (stack_length(stack) < 1) {
    printf("EVM - Stack Underflow\n");
  } else {
    if (index == 0) {
      Node *node = stack->first;
      stack->first = stack->first->next;
      uint256_t *data = node->data;
      Node_destroy(node);

    } else {
      Node *before = stack->first;
      while (index > 1) {
        before = before->next;
        --index;
      }
      Node *node = before->next;
      before->next = before->next->next;
      uint256_t *data = node->data;
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
    printf("STACK\n");
    while (node->next != NULL) {

      printf("0x%03X 0x%016llX%016llX%016llX%016llX", i, node->data->e[0],
             node->data->e[1], node->data->e[2], node->data->e[3]);
      --i;
      node = node->next;
      if (node->next != NULL) {
        printf("\n");
      }
    }
    printf("\n");
  }
}

// printf specific stack index
// @param stack: which to stack to point to
// @param index: address to print from stack
void stack_peak_print(List *stack, int index) {
  assert(stack != NULL);
  uint256_t *val = stack_peak(stack, index);
  printf("STACK PEEK\n0x%03X 0x%016llX%016llX%016llX%016llX\n", index, E_0(val),
         E_1(val), val->e[2], val->e[3]);
}
