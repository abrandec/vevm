#include "main_t.h"
#include "../src/common/cmd/cmd.h"
#include "../src/common/utils/hex_utils/hex_utils.h"
#include "common_t/math_t/bigint_t/bigint_t.h"
#include "core_t/stack_t/stack_t.h"
#include "core_t/vm_t/vm_t.h"
#include "test_utils/assert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  // number of tests
  enum { test_len = 2 };

  bool test_results[test_len] = {stack_tests(), vm_tests()};

  // check if all test results passed
  assert_bool_array_msg("Final test results", test_results, test_len);

  return 0;
}

