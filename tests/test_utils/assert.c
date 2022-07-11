
#include "assert.h"

#include "../../src/common/cmd/cmd.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>

bool assert_msg(char msg[], bool condition) {
  if (condition) {
    printf(GREEN "│ ▪ %s - PASS\n" RESET, msg);
    return true;
  } else {
    printf(RED "│ ▪ %s - FAIL\n" RESET, msg);
    return false;
  }
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

bool assert_eq_msg(char msg[], uint64_t a, uint64_t b) {
  return assert_msg(msg, a == b);
}

bool assert_eq_128_msg(char msg[], uint128_t a, uint128_t b) {
  return assert_msg(msg, equal_uint128(&a, &b));
}

bool assert_eq_256_msg(char msg[], uint256_t a, uint256_t b) {
  return assert_msg(msg, equal_uint256(&a, &b));
}

bool assert_eq_512_msg(char msg[], uint512_t a, uint512_t b) {
  return assert_msg(msg, equal_uint512(&a, &b));
}

bool assert_char(char *a, char *b) { return (strcmp(a, b) == 0); }

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
      printf(RED "├──────────────────────────────────\n│ ▸ %s - "
                 "FAIL\n├──────────────────────────────────\n" RESET,
             msg);

      return false;
    }
  }

  printf(GREEN "├──────────────────────────────────\n│ ▸ %s - "
               "PASS\n├──────────────────────────────────\n" RESET,
         msg);

  return true;
}
