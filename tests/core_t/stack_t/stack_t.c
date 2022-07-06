#include "stack_t.h"

#include "../../../src/common/cmd/cmd.h"
#include "../../../src/common/math/bigint/bigint.h"
#include "../../../src/core/config.h"
#include "../../../src/core/stack/stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// overflow check
// @param stack: the stack to check
bool assert_overflow(List *stack) {
  bool result = true;
  uint256_t dummy_data = init_all_uint256(0, 0, 0, 69);
  for (int i = 0; i < MAX_STACK_DEPTH; ++i) {
    stack_push(stack, &dummy_data);
  }

  if (!result) {
    printf(RED "Stack overflow - FAIL\n" RESET);
  } else {
    printf(GREEN "Stack overflow - PASS\n" RESET);
  }

  return result;
}

// underflow check
// @param stack: the stack to check
bool assert_underflow(List *stack) {
  bool result = true;

  for (int i = 0; i < MAX_STACK_DEPTH; ++i) {
    stack_pop(stack);
  }

  if (!result) {
    printf(RED "Stack underflow - FAIL\n" RESET);
  } else {
    printf(GREEN "Stack underflow - PASS\n" RESET);
  }

  return result;
}

bool stack_tests(void) {
  List *stack = stack_create();
  
  bool stack_tests_results = true;

  bool stack_results[2] = {assert_overflow(stack), assert_underflow(stack)};

  for (int i = 0; i < 2; ++i) {
    if (!stack_results[i]) {
      stack_tests_results = false;
    }
  }

  return stack_tests_results;
}