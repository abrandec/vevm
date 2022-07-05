#include "../../src/bigint/bigint.h"
#include "../../src/stack/stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_DEPTH 10

// assert that stack depth does not exceed MAX_STACK_DEPTH
// @param stack: the stack to check
// @param dummy: dummy data to push onto the stack
// @return true if stack depth does not exceed MAX_STACK_DEPTH, false otherwise
bool assert_stack_overflow(List *stack, uint256_t *dummy) {

  for (int i = 0; i < MAX_STACK_DEPTH + 1; ++i) {
    stack_push(stack, dummy);
  }
  if (stack_length(stack) == MAX_STACK_DEPTH) {
    printf("assert_stack_max - pass\n");
  } else if (stack_length(stack) > MAX_STACK_DEPTH) {
    printf("assert_stack_max - fail\n");
  }
}

// assert that stack depth does not underflow
// @param stack: the stack to check
// @return true if stack depth does not underflow, false otherwise
bool assert_stack_underflow(List *stack) {
  stack_pop(stack);

  if (stack_length(stack) == 0) {
    printf("assert_stack_underflow - pass\n");
  } else {
    printf("assert_stack_underflow - fail\n");
  }
}

// assert that stack can be popped
// @param stack: the stack to check
// @return true if stack can be popped, false otherwise
bool assert_pop(List *stack) {
  int stack_len = stack_length(stack);

  stack_pop(stack);

  if (stack_len == stack_length(stack) + 1) {
    printf("assert_pop - pass\n");
  } else {
    printf("assert_pop - fail\n");
  }
}

// assert that stack can be peaked and data is valid
// @param stack: the stack to check
// @param dummy: dummy data to compare to the peaked data
// @param index: the index of the stack to peek at
// @return true if stack can be peaked and data is valid, false otherwise
bool assert_peak(List *stack, uint256_t *dummy, int index) {
  uint256_t *val = stack_peak(stack, index);
  if (val == dummy) {
    printf("assert_peak - pass\n");
  } else {
    printf("assert_peak - fail\n");
  }
}

// main function
int main(void) {
  List *stack = stack_create();
  uint256_t *x;
  clear_uint256(x);

  // fill entire uint256_t with 1's
  E_0(x).e[0] = 0xFFFFFFFFFFFFFFFF;
  E_0(x).e[1] = 0xFFFFFFFFFFFFFFFF;
  E_1(x).e[0] = 0xFFFFFFFFFFFFFFFF;
  E_1(x).e[1] = 0xFFFFFFFFFFFFFFFF;

  // basic tests
  assert_stack_underflow(stack);
  assert_stack_overflow(stack, x);
  assert_pop(stack);
  assert_peak(stack, x, 0);
  stack_destroy(stack);

  return 0;
}