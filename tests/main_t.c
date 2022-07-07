#include "main_t.h"
#include "../src/common/cmd/cmd.h"
#include "../src/common/utils/hex_utils/hex_utils.h"
#include "common_t/math_t/bigint_t/bigint_t.h"
#include "core_t/stack_t/stack_t.h"
#include "core_t/vm_t/vm_t.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool stack_overflow_bool = true;
static bool stack_underflow_bool = true;
static bool stack_out_of_bounds_bool = true;

int main(void) {
  bool all_results = true;

  bool test_results[2] = {stack_tests()};

  printf("Stack tests: %s\n",
         test_results[0] ? GREEN "PASS" RESET : RED "FAIL" RESET);

  //////////////////////////////
  // Final test results check //
  //////////////////////////////

  for (int i = 0; i < 1; ++i) {
    if (!test_results[i]) {
      all_results = false;
      printf("Final test results: %s\n",
             all_results ? GREEN "PASS" RESET : RED "FAIL" RESET);
      return 0;
    }
  }

  all_results = true;
  printf("Final test results: %s\n",
         all_results ? GREEN "PASS" RESET : RED "FAIL" RESET);
}