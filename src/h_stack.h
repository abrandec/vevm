#ifndef h_stack_H_
#define h_stack_H_

// Heap based stack

#include "uint256.h"

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

void stack_pop(List *stack);

uint256_t *stack_peak(List *stack, int index);

int stack_length(List *stack);

// printf functions

void stack_print(List *stack);

void stack_peak_print(List *stack, int index);

#endif