#ifndef STACK_H
#define STACK_H

#include "../../common/math/bigint/bigint.h"

/*
  ┌───────────────────────────────┐
  │   TYPE DEFINITIONS            │
  └───────────────────────────────┘
 */

typedef struct NodeTag {
  uint256_t *data;
  struct NodeTag *next;
} Node;

typedef struct ListTag {
  struct NodeTag *first;
} List;

/*
  ┌───────────────────────────────┐
  │   ERRORS                      │
  └───────────────────────────────┘
 */

static const char stack_null_err[14] = "Stack is NULL\n";
static const char inaccessable_element_err[36] = "EVM - Stack element is inaccessable\n";
static const char stack_overflow_err[22] = "EVM - Stack Overflow\n";
static const char stack_underflow_err[22] = "EVM - Stack Underflow\n";

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   NODE FUNCTIONS                                                           │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   NODE CREATE                 │
  └───────────────────────────────┘
 */

// create a new node
Node *Node_create(void);

/*
  ┌───────────────────────────────┐
  │   NODE DESTROY                │
  └───────────────────────────────┘
 */

// destroy a node
// @param node: the node to destroy
void Node_destroy(Node *node);

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   STACK FUNCTIONS                                                          │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   STACK CREATE                │
  └───────────────────────────────┘
 */

// create a new stack
// @return a new stack
List *stack_create(void);

/*
  ┌───────────────────────────────┐
  │   STACK DESTROY               │
  └───────────────────────────────┘
 */

// destroy the stack
// @param stack: the stack to destroy
void stack_destroy(List *stack);

/*
  ┌───────────────────────────────┐
  │   STACK PUSH                  │
  └───────────────────────────────┘
 */

// push a new element on top of the stack
// @param stack: the stack to push onto
// @param data: the data to push onto the stack
void stack_push(List *stack, uint256_t *data);

/*
  ┌───────────────────────────────┐
  │   STACK POP                   │
  └───────────────────────────────┘
 */

// pop the top element on the stack
// @param stack: the stack to pop the top element from
void stack_pop(List *stack);

/*
  ┌───────────────────────────────┐
  │   STACK SWAP                  │
  └───────────────────────────────┘
 */

// swap the top element with another element
// @param stack: the stack to swap the top element of
// @param index: the index of the element to swap with
void stack_swap(List *stack, int index);

/*
  ┌───────────────────────────────┐
  │   STACK PEAK                  │
  └───────────────────────────────┘
 */

// peak at the stack at a certain index
// @param stack: stack to peek from
// @param index: index of the stack to peek
// @return the stack item
uint256_t stack_peak(List *stack, int index);

/*
  ┌───────────────────────────────┐
  │   STACK LENGTH                │
  └───────────────────────────────┘
 */

// gets length of stack
// @param stack: stack to get length of
// @return length of stack
int stack_length(List *stack);

#endif