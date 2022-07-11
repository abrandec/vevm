// Tests
#include "common_t/io_t/io_t.h"
#include "common_t/math_t/bigint_t/bigint_t.h"
#include "core_t/stack_t/stack_t.h"
#include "core_t/vm_t/vm_t.h"
// End of Tests

#include "test_utils/assert.h"

#include "../src/common/cmd/argparse/argparse.h"
#include "../src/common/cmd/cmd.h"
#include "../src/common/utils/hex_utils/hex_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const static char vevm_logo[93] =
    "\033[7m\e[0;37mvevm test suite "
    "\033[27m\033[93m▓▓\033[94m▓▓\033[92m▓▓\033[35m▓▓\033[91m▓▓\033[00m\0";

// argparse
static const char *const usages[] = {
    vevm_logo,
    NULL,
};

int main(int argc, const char *argv[]) {

  // param variables
  int _all_tests = 0;
  int _bigint_tests = 0;
  int _stack_tests = 0;
  int _vm_tests = 0;
  int _io_tests = 0;

  struct argparse_option options[] = {
      OPT_GROUP("Commands"),
      OPT_BOOLEAN('a', "all", &_all_tests, "Run all tests", NULL, 0, 0),
      OPT_BOOLEAN('b', "bigint", &_bigint_tests, "Run bigint tests", NULL, 0, 0),
      OPT_BOOLEAN('s', "stack", &_stack_tests, "Run stack tests", NULL, 0, 0),
      OPT_BOOLEAN('v', "vm", &_vm_tests, "Run vm tests", NULL, 0, 0),
      OPT_BOOLEAN('i', "io", &_io_tests, "Run io tests", NULL, 0, 0),
      OPT_HELP(),
      OPT_END(),
  };

  // parse arguments
  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argc = argparse_parse(&argparse, argc, argv);

  // ┌───────────────────┐
  // │   ALL TESTS       │
  // └───────────────────┘
  if (_all_tests != 0) {
    enum { test_len = 4 };
    bool test_results[test_len] = {bigint_tests(), stack_tests(), vm_tests(), io_tests()};
    assert_bool_array_msg("Final test results", test_results, test_len);
  }

  // ┌───────────────────┐
  // │   STACK TESTS     │
  // └───────────────────┘
  if (_bigint_tests != 0) {
    enum { test_len = 1 };
    bool test_results[test_len] = {bigint_tests()};
    assert_bool_array_msg("Final test results", test_results, test_len);
  }

  // ┌───────────────────┐
  // │   STACK TESTS     │
  // └───────────────────┘
  if (_stack_tests != 0) {
    enum { test_len = 1 };
    bool test_results[test_len] = {stack_tests()};
    assert_bool_array_msg("Final test results", test_results, test_len);
  }

  // ┌───────────────────┐
  // │   VM TESTS        │
  // └───────────────────┘
  if (_vm_tests != 0) {
    enum { test_len = 1 };
    bool test_results[test_len] = {vm_tests()};
    assert_bool_array_msg("Final test results", test_results, 1);
  }

  // ┌───────────────────┐
  // │   IO TESTS        │
  // └───────────────────┘
  if (_io_tests != 0) {
    enum { test_len = 1 };
    bool test_results[test_len] = {io_tests()};
    assert_bool_array_msg("Final test results", test_results, test_len);
  }

  // ┌───────────────────────────────────┐
  // │   IF NO COMMAND EXISTS            │
  // └───────────────────────────────────┘
  if (argv != 0 && _io_tests == 0 && _vm_tests == 0 && _stack_tests == 0 && _bigint_tests == 0 && _all_tests == 0) {
    argparse_usage(&argparse);
  }

  return 0;
}
