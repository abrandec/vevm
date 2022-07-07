#include "stack_t.h"

#include "../../../src/common/cmd/cmd.h"
#include "../../../src/common/math/bigint/bigint.h"
#include "../../../src/core/config.h"
#include "../../../src/core/stack/stack.h"
#include "../../../src/errors/errors.h"
#include "../../main_t.h"
#include "../../test_utils/assert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// assert that the stack can handle overflows
// @param stack: the stack to check
// @return: true if the stack can handle overflows, false otherwise
bool assert_overflow(List *stack) {
  uint256_t dummy_data = init_all_uint256(0, 0, 0, 69);

  for (int i = 0; i < MAX_STACK_DEPTH + 1; ++i) {
    stack_push(stack, &dummy_data);
  }

  return assert_eq_msg("Overflow check", stack_length(stack), MAX_STACK_DEPTH);
}

// assert that the stack can handle underflows
// @param stack: the stack to check
// @return: true if the stack can handle underflows, false otherwise
bool assert_underflow(List *stack) {
  for (int i = 0; i < MAX_STACK_DEPTH + 1; ++i) {
    stack_pop(stack);
  }

  return assert_eq_msg("Underflow check", stack_length(stack), 0);
}

// assert that stack elements can be swapped
// @param stack: the stack to check
// @return true if stack elements can be swapped, false otherwise
bool assert_swap(List *stack) {
  enum { max_stack_depth = MAX_STACK_DEPTH };

  rand_num_uint128(init_all_uint128(0, 0), init_all_uint128(0, 20));
  uint256_t dummy_data[max_stack_depth] = {};

  // final suffle dummy data
  uint256_t final_suffle_dd[max_stack_depth] = {};

  dummy_data[0] = init_all_uint256(0, 0, 0, 420);
  dummy_data[0] = init_all_uint256(0, 0, 0, 69);

  // do some suffling on the stack
  stack_push(stack, &dummy_data[0]);

  stack_push(stack, &final_suffle_dd[0]);

  stack_swap(stack, 1);

  return assert_eq_256_msg("Swap check", stack_peak(stack, 0), dummy_data[0]);
}

// assert that a stack can be destroyed
// @param stack: the stack to check
// @return true if the stack can be destroyed, false otherwise
bool assert_stack_destroy(List *stack) {
  uint256_t dummy_data = init_all_uint256(0, 0, 0, 69);

  // check if stack is empty/zero in the first place and if so
  // add an element to the stack to make it non-empty
  stack_length(stack) == 0 ? stack_push(stack, &dummy_data) : 0;

  stack_destroy(stack);

  return assert_eq_msg("Stack destroy check", stack_length(stack), 0);
}

// main stack test
// @return true if all tests pass, false otherwise
bool stack_tests(void) {
  List *stack = stack_create();

  // number of tests
  enum { stack_l = 4 };

  // run all tests
  // assert_stack_destroy for last test
  bool all_tests[stack_l] = {assert_overflow(stack), assert_underflow(stack),
                             assert_swap(stack), assert_stack_destroy(stack)};

  // check & return results
  return assert_bool_array_msg("Stack tests", all_tests, stack_l);
}