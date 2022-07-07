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

// overflow check
// @param stack: the stack to check
bool assert_overflow(List *stack) {

  uint256_t dummy_data = init_all_uint256(0, 0, 0, 69);
  for (int i = 0; i < MAX_STACK_DEPTH + 1; ++i) {
    stack_push(stack, &dummy_data);
  }

  return assert_eq_msg("Overflow check", stack_length(stack), MAX_STACK_DEPTH);
}

// underflow check
// @param stack: the stack to check
bool assert_underflow(List *stack) {

  for (int i = 0; i < MAX_STACK_DEPTH + 1; ++i) {
    stack_pop(stack);
  }

  return assert_eq_msg("Underflow check", stack_length(stack), 0);
}

// main test
bool stack_tests(void) {
  List *stack = stack_create();
  enum { stack_t_l = 2 };
  bool stack_tests_results = true;

  bool stack_results[stack_t_l] = {assert_overflow(stack),
                                 assert_underflow(stack)};

  for (int i = 0; i < stack_t_l; ++i) {
    if (!stack_results[i]) {
      stack_tests_results = false;
    }
  }

  return stack_tests_results;
}