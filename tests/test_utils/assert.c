
#include "assert.h"

#include "../../src/common/cmd/cmd.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool assert_msg(char msg[], bool condition) {
  if (condition) {
    printf(GREEN "│ %s - PASS\n" RESET, msg);
    return true;
  } else {
    printf(RED "│ %s - FAIL\n" RESET, msg);
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

uint64_t rand_num(uint64_t min, uint64_t max) {
  time_t t;
  srand((unsigned)time(&t));
  return (rand() % (max - min + 1)) + min;
}

uint128_t rand_num_uint128(uint128_t min, uint128_t max) {
  time_t t;

  srand((unsigned)time(&t));

  uint128_t result;
  uint128_t temp1;
  uint128_t temp2;
  uint128_t temp3;

  uint128_t one;
  E1(one) = 1;
  uint128_t randomNum;

  E0(randomNum) = rand();
  E1(randomNum) = rand();

  sub_uint128(&temp1, &max, &min);
  add_uint128(&temp1, &temp1, &one);
  divmod_uint128(&temp3, &temp2, &result, &randomNum);
  add_uint128(&result, &result, &min);

  return result;
}

uint256_t rand_num_uint256(uint256_t min, uint256_t max) {
  time_t t;

  srand((unsigned)time(&t));

  uint256_t result;
  uint256_t temp1;
  uint256_t temp2;
  uint256_t temp3;

  uint256_t one;
  E11(one) = 1;
  uint256_t randomNum;

  change_uint256(&randomNum, rand(), rand(), rand(), rand());

  sub_uint256(&temp1, &max, &min);
  add_uint256(&temp1, &temp1, &one);
  divmod_uint256(&temp3, &temp2, &result, &randomNum);
  add_uint256(&result, &result, &min);

  return result;
}

uint512_t rand_num_uint512(uint512_t min, uint512_t max) {
  time_t t;

  srand((unsigned)time(&t));

  uint512_t result;
  uint512_t temp1;
  uint512_t temp2;
  uint512_t temp3;

  uint512_t one;
  change_uint512(&one, 0, 0, 0, 0, 0, 0, 0, 1);
  uint512_t randomNum;
  change_uint512(&randomNum, rand(), rand(), rand(), rand(), rand(), rand(),
                 rand(), rand());

  sub_uint512(&temp1, &max, &min);
  add_uint512(&temp1, &temp1, &one);
  divmod_uint512(&temp3, &temp2, &result, &randomNum);
  add_uint512(&result, &result, &min);

  return result;
}

bool assert_bool_array_msg(char msg[], bool arr[], size_t len) {
  int i = 0;

  for (; i < len; ++i) {
    if (!arr[i]) {
      printf(RED "├──────────────────────────────────\n│ %s - FAIL\n" RESET, msg);

      return false;
    }
  }

  printf(GREEN "├──────────────────────────────────\n│ %s - PASS\n" RESET, msg);

  return true;
}
