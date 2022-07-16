
#include "assert.h"

#include "../../src/common/cmd/cmd.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>

#pragma GCC diagnostic ignored "-Wformat"

bool assert_msg(const char msg[], bool condition) {
  if (condition) {
    printf(GREEN "│ ▸ %s - PASS\n" RESET, msg);
    return true;
  } else {
    printf(RED "│ ▸ %s - FAIL\n" RESET, msg);
    return false;
  }
}

void assert_pass(const char function[]) {
  printf(GREEN "│ ▸ %s - PASS\n" RESET, function);
}

bool set_error(error_t *error, const char main_func[], char msg[], uint8_t data_type, bool base10, bool _signed) {
  error->main_function = main_func;
  error->message = msg;
  error->data_type = data_type;
  error->base10 = base10;
  error->_signed = _signed;

  return true;
}

bool assert_error(error_t *error) {
  if (error->data_type == ASSERT_UINT128_T) {
    uint128_t expected = *(uint128_t *)error->expected;
    uint128_t actual = *(uint128_t *)error->actual;

    printf((error->base10) ? RED "│ ▹ %s - %s\n"
                               "│   ├─ expected: "
                               "" DEC_UINT128 "\n│ "
                               "  └─ actual:   " DEC_UINT128 "\n" RESET
                         : RED "│ ▹ %s - %s\n"
                               "│   ├─ expected: "
                               "0x" HEX_UINT128 "\n│ "
                               "  └─ actual:   0x" HEX_UINT128 "\n" RESET,
           error->main_function, error->message, UINT128(expected),
           UINT128(actual));
    return false;
  } else if (error->data_type == ASSERT_UINT256_T) {
    uint256_t expected = *(uint256_t *)error->expected;
    uint256_t actual = *(uint256_t *)error->actual;

    printf((error->base10) ? RED "│ ▹ %s - %s\n"
                               "│   ├─ expected: "
                               "" DEC_UINT256 "\n│ "
                               "  └─ actual:   " DEC_UINT256 "\n" RESET
                         : RED "│ ▹ %s - %s\n"
                               "│   ├─ expected: "
                               "0x" HEX_UINT256 "\n│ "
                               "  └─ actual:   0x" HEX_UINT256 "\n" RESET,
           error->main_function, error->message, UINT256(expected),
           UINT256(actual));
    return false;
  } else if (error->data_type == ASSERT_UINT512_T) {
    uint512_t expected = *(uint512_t *)error->expected;
    uint512_t actual = *(uint512_t *)error->actual;

    printf((error->base10) ? RED "│ ▹ %s - %s\n"
                               "│   ├─ expected: "
                               "" DEC_UINT512 "\n│ "
                               "  └─ actual:   " DEC_UINT512 "\n" RESET
                         : RED "│ ▹ %s - %s\n"
                               "│   ├─ expected: "
                               "0x" HEX_UINT512 "\n│ "
                               "  └─ actual:   0x" HEX_UINT512 "\n" RESET,
           error->main_function, error->message, UINT512(expected),
           UINT512(actual));
    return false;
  } else if (error->data_type == ASSERT_BOOL_T) {
    return false;
  } else if (error->data_type == ASSERT_CHAR_T) {
    return false;
  } else if (error->data_type == ASSERT_UINT8_T) {
    return false;
  } else if (error->data_type == ASSERT_UINT8_T) {
    return false;
  } else if (error->data_type == ASSERT_FLOAT_T) {
    return false;
  }
  return false;
}

bool assert_eq(uint64_t a, uint64_t b) { return (a == b); }

bool assert_eq_uint128(uint128_t a, uint128_t b) {
  return (equal_uint128(&a, &b));
}

bool assert_eq_uint256(uint256_t a, uint256_t b) {
  return (equal_uint256(&a, &b));
}

bool assert_eq_uint512(uint512_t a, uint512_t b) {
  return (equal_uint512(&a, &b));
}

bool assert_eq_msg(const char func[], const char msg[], uint64_t expected,
                   uint64_t actual) {
  if (expected == actual) {
    printf(GREEN "│ ▸ %s - pass\n" RESET, func);
    return true;
  } else {
    printf(RED "│   \n│ ▸ %s - fail\n│   %s\n│   ├─ expected: %020lld\n│ "
               "  └─ actual:   %020lld\n" RESET,
           func, msg, expected, actual);
    return false;
  }
}

bool assert_eq_128_msg(char msg[], uint128_t a, uint128_t b) {
  return assert_msg(msg, equal_uint128(&a, &b));
}

bool assert_eq_256_msg(const char func[], const char msg[], uint256_t expected,
                       uint256_t actual) {
  if (equal_uint256(&expected, &actual)) {
    printf(GREEN "│ ▸ %s - pass\n" RESET, func);
    return true;
  } else {
    printf(RED "│   \n│ ▸ %s - fail\n│   %s\n│   ├─ expected: "
               "0x" HEX_UINT256 "\n│ "
               "  └─ actual:   0x" HEX_UINT256 "\n" RESET,
           func, msg, UINT256(expected), UINT256(actual));
    return false;
  }
}

bool assert_eq_512_msg(char msg[], uint512_t a, uint512_t b) {
  return assert_msg(msg, equal_uint512(&a, &b));
}

bool assert_char(const char func[], const char msg[], char *expected,
                 char *actual) {
  if (strcmp(expected, actual) == 0) {
    printf(GREEN "│ ▸ %s - pass\n" RESET, func);
    return true;
  } else {
    printf(RED "│   \n│ ▸ %s - fail\n│   %s\n│   ├─ expected: %s\n│ "
               "  └─ actual:   %s\n" RESET,
           func, msg, expected, actual);
    return false;
  }
}

uint64_t rand_num(void) {
  unsigned int tmp;

  getrandom(&tmp, sizeof(unsigned int), GRND_NONBLOCK) == -1
      ? perror("getrandom")
      : "";

  return tmp;
}

bool assert_bool_array_msg(char msg[], bool arr[], size_t len) {
  int i = 0;

  for (; i < len; ++i) {
    if (!arr[i]) {
      printf(RED "├──────────────────────────────────\n│ %s - "
                 "FAIL\n├──────────────────────────────────\n" RESET,
             msg);

      return false;
    }
  }

  printf(GREEN "├──────────────────────────────────\n│ %s - "
               "PASS\n├──────────────────────────────────\n" RESET,
         msg);

  return true;
}
