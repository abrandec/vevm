#include "bigint_t.h"

#include "../../../../src/common/math/bigint/bigint.h"
#include "../../../test_utils/assert.h"

#include "types/uint128_t.h"
#include "types/uint256_t.h"
#include "types/uint512_t.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool bigint_tests(void) {

  // number of tests
  enum { stack_l = 0 };

  // run all tests
  // assert_stack_destroy for last test
  bool all_tests[stack_l] = {};

  // check & return results
  return assert_bool_array_msg("Big int tests", all_tests, stack_l);
}