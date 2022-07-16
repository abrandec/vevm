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

const static char incorrect_value[20] = "incorrect value\0";
const static char incorrect_len[20] = "incorrect length\0";
const static char max_stack_dep[35] = "swapped passed max stack depth\0";

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
      return assert_eq_256_msg(__func__, incorrect_value, dummy_data, data);
    }
  }

  stack_push(&dummy_data);
  stack_push(&dummy_data);
  return assert_eq_msg(__func__, incorrect_len, MAX_STACK_DEPTH,
                       stack_length());
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
      return assert_eq_256_msg(__func__, incorrect_value, data, data2);
    }

    // assert length
    if (stack_length() != MAX_STACK_DEPTH - (i + 1)) {
      return assert_eq_msg(__func__, incorrect_len, MAX_STACK_DEPTH - (i + 1),
                           stack_length());
    }
  }

  return assert_eq_msg(__func__, incorrect_value, 0, stack_length());
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

    if (!equal_uint256(&data_i, &data_top2) ||
        !equal_uint256(&data_top, &data_i2)) {
      return assert_eq_256_msg(__func__, incorrect_value, data_i, data_top2);
    }


    if (!equal_uint256(&data_top, &data_i2)) {
      return assert_eq_256_msg(__func__, incorrect_value, data_top, data_i2);
    }
  }
  
  uint256_t end_of_stack = stack_peak(max_stack_depth + 1);

  stack_swap(max_stack_depth + 1);

  uint256_t end_of_stack2 = stack_peak(max_stack_depth + 1);

  return assert_eq_256_msg(__func__, max_stack_dep, end_of_stack, end_of_stack2);
}

// main stack test
// @return true if all tests pass, false otherwise
bool stack_tests(void) {

  // number of tests
  enum { stack_l = 3 };

  // run all tests
  bool all_tests[stack_l] = {assert_overflow(),
                             assert_underflow(),
                             assert_swap()};

  // check & return results
  return assert_bool_array_msg("STACK TESTS", all_tests, stack_l);
}