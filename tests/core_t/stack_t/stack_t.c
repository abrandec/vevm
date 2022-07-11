#include "stack_t.h"

#include "../../../src/common/cmd/cmd.h"
#include "../../../src/common/math/bigint/bigint.h"
#include "../../../src/core/config.h"
#include "../../../src/core/stack/stack.h"
#include "../../../src/errors/errors.h"
#include "../../test_utils/assert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// assert that the stack can handle overflows
// @return: true if the stack can handle overflows, false otherwise
bool assert_overflow(void) {
  uint256_t dummy_data = init_uint256(rand_num());

  for (int i = 0; i < MAX_STACK_DEPTH - 1; ++i) {
    change_all_uint256(&dummy_data, rand_num(), rand_num(), rand_num(),
                       rand_num());

    stack_push(&dummy_data);
    uint256_t data = stack_peak(stack_length() - 1);

    // compare data
    if (!equal_uint256(&data, &dummy_data)) {
      return assert_msg("Overflow check", false);
    }
  }

  stack_push(&dummy_data);

  return assert_eq(stack_length(), MAX_STACK_DEPTH);
}

// assert that the stack can handle underflows
// @return: true if the stack can handle underflows, false otherwise
bool assert_underflow(void) {
  int i;
  for (i = 0; i < MAX_STACK_DEPTH; ++i) {
    // check that the stack is actually LIFO
    uint256_t data = stack_peak(i);

    stack_pop();

    uint256_t data2 = stack_peak(i);

    // compare data
    if (!equal_uint256(&data, &data2)) {
      return assert_msg("Stack is not LIFO", false);
    }

    // assert length
    if (stack_length() != MAX_STACK_DEPTH - (i + 1)) {
      return assert_msg("Incorrect length", false);
    }
  }

  return assert_eq(stack_length(), 0);
}

// assert that stack elements can be swapped
// @return true if stack elements can be swapped, false otherwise
bool assert_swap(void) {
  enum { max_stack_depth = MAX_STACK_DEPTH };
  uint256_t dummy_data[max_stack_depth] = {};

  int i;
  for (i = 0; i < max_stack_depth; ++i) {
    dummy_data[i] = init_uint256(rand_num());
    stack_push(&dummy_data[i]);
  }

  // max stack swap is 16 elements
  // could be a better test but whatever, seems to work in the EVM
  for (i = 0; i < max_stack_depth - 1; ++i) {

    uint256_t data_i = stack_peak(i);
    uint256_t data_top = stack_peak(max_stack_depth - 1);

    stack_swap(i);

    uint256_t data_i2 = stack_peak(i);
    uint256_t data_top2 = stack_peak(max_stack_depth - 1);

    if (!equal_uint256(&data_i, &data_top2) || !equal_uint256(&data_top, &data_i2)) {
      return assert_msg("Data not equal", false);
    }


  }

  stack_swap(max_stack_depth + 1);

  return true;
}

// main stack test
// @return true if all tests pass, false otherwise
bool stack_tests(void) {

  // number of tests
  enum { stack_l = 3 };

  // run all tests
  // assert_stack_destroy for last test
  bool all_tests[stack_l] = {assert_msg("Overflow", assert_overflow()),
                             assert_msg("Underflow", assert_underflow()),
                             assert_msg("Swap", assert_swap())};

  // check & return results
  return assert_bool_array_msg("Stack tests", all_tests, stack_l);
}