#ifndef h_stack_H_
#define h_stack_H_

// Heap based stack

#include "uint256.c"
#include "uint256.h"

#define MAX_STACK_DEPTH 10

// get element at the last index of the stack
// @param stack: the stack to get the last element from
// @return the last element of the stack
#define STACK_TOP(x) stack_length(x) - 1

typedef struct NodeTag {
  uint256_t *data;
  struct NodeTag *next;
} Node;

typedef struct ListTag {
  struct NodeTag *first;
} List;

Node *Node_create(void);

void Node_destroy(Node *node);

// standard stack functions

List *stack_create(void);

void stack_destroy(List *stack);

void stack_push(List *stack, uint256_t *data);

void stack_swap(List *stack, int index);

void stack_pop(List *stack);

uint256_t stack_peak(List *stack, int index);

int stack_length(List *stack);

// printf functions

void stack_print(List *stack);

void stack_peak_print(List *stack, int index);

#endif