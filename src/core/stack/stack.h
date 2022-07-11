#ifndef STACK_H
#define STACK_H

#include "../../common/math/bigint/bigint.h"

/*
  ┌───────────────────────────────┐
  │   STACK PUSH                  │
  └───────────────────────────────┘
 */

// push a new element on top of the stack
// @param data: the data to push onto the stack
void stack_push(uint256_t *data);

/*
  ┌───────────────────────────────┐
  │   STACK POP                   │
  └───────────────────────────────┘
 */

// pop the top element on the stack
void stack_pop();

/*
  ┌───────────────────────────────┐
  │   STACK SWAP                  │
  └───────────────────────────────┘
 */

// swap the top element with another element
// @param index: the index of the element to swap with
void stack_swap(int index);

/*
  ┌───────────────────────────────┐
  │   STACK PEAK                  │
  └───────────────────────────────┘
 */

// peak at the stack at a certain index
// @param index: index of the stack to peek
// @return the stack item
uint256_t stack_peak(int index);

/*
  ┌───────────────────────────────┐
  │   STACK LENGTH                │
  └───────────────────────────────┘
 */

// gets length of stack
// @return length of stack
int stack_length();

#endif