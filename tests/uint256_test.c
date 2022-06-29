#include "../src/uint256.c"
#include "../src/uint256.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// assert copying of a uint128_t
// @param dest: the destination to copy the data to
// @param data: the data to copy
// @return true if the copy was successful, false otherwise
bool assert_copy_uint128(uint128_t dest, uint128_t data) {
  copy_uint128(dest, data);
  if (E0(dest) != E0(data) && E1(dest) != E1(data)) {
    printf("copy_uint128 failed\n");
    printf("dest: ");
    print_hex_uint128(dest);
    printf("data: ");
    print_hex_uint128(data);
    return false;
  } else {

    printf("copy_uint128 - Passed\n");
    return true;
  }
}

// assert copying of a uint256_t
// @param dest: the destination to copy the data to
// @param data: the data to copy
// @return true if the copy was successful, false otherwise
bool assert_copy_uint256(uint256_t dest, uint256_t *data) {
  copy_uint256(dest, data);
  if (E0(dest).e[0] != E0(data).e[0] && E0(dest).e[1] != E0(data).e[1] &&
      E1(dest).e[0] != E1(data).e[0] && E1(dest).e[1] != E1(data).e[1]) {
    printf("copy_uint256 failed\n");
    printf("dest: ");
    print_hex_uint256(dest);
    printf("data: ");
    print_hex_uint256(data);
    return false;
  } else {

    printf("copy_uint256 - Passed\n");
    return true;
  }
}

// assert that a uint128_t is cleared to zero
// @param dest: the uint128 to clear
// @return true if the uint128 is cleared to zero, false otherwise
bool assert_clear_uint128(uint128_t *dest) {
  clear_uint128(dest);
  if (E0(dest) != 0 && E1(dest) != 0) {
    printf("clear_uint128 failed\n");
    printf("result: ");
    print_hex_uint128(dest);
    return false;
  } else {

    printf("clear_uint128 - Passed\n");
    return true;
  }
}

// assert that a uint256_t is cleared to zero
// @param dest: the uint256_t to clear
// @return true if the uint256_t is cleared to zero, false otherwise
bool assert_clear_uint256(uint256_t *dest) {
  clear_uint256(dest);
  if (E0(dest).e[0] != 0 && E0(dest).e[1] != 0 && E1(dest).e[0] != 0 &&
      E1(dest).e[1] != 0) {
    printf("clear_uint256 failed\n");
    printf("result: ");
    print_hex_uint256(dest);
    return false;
  } else {

    printf("clear_uint256 - Passed\n");
    return true;
  }
}

// assert that two uint128_ts are equal
// @param a: first uint128_t
// @param b: second uint128_t
// @return true if the uint128s are equal, false otherwise
bool assert_equal_uint128(uint128_t *a, uint128_t *b) {
  if (!equal_uint128(a, b)) {
    printf("equal_uint128 failed\n");
    printf("a: ");
    print_hex_uint128(a);
    printf("b: ");
    print_hex_uint128(b);

    return false;
  } else {

    printf("equal_uint128 - Passed\n");
    return true;
  }
}

// assert that two uint256_ts are equal
// @param a: first uint256_t
// @param b: second uint256_t
// @return true if equal, false if not
bool assert_equal_uint256(uint256_t *a, uint256_t *b) {
  if (!equal_uint256(a, b)) {
    printf("equal_uint128 failed\n");
    printf("a: ");
    print_hex_uint256(a);
    printf("b: ");
    print_hex_uint256(b);
    return false;
  } else {
    printf("equal_uint256 - Passed\n");
    return true;
  }
}

// assert adding 2 uint128_ts
// @param dest: destination for result
// @param a: first uint128_t to add
// @param b: second uint128_t to add
// @return true if addition is correct, false otherwise
bool assert_add_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  add_uint128(dest, a, b);
  if (E0(dest) != E0(a) + E0(b) && E1(dest) != E1(a) + E1(b)) {
    printf("add_uint128 failed\n");
    printf("dest: ");
    print_hex_uint128(dest);
    printf("a: ");
    print_hex_uint128(a);
    printf("b: ");
    print_hex_uint128(b);
    return false;
  } else {

    printf("add_uint128 - Passed\n");
    return true;
  }
}

// assert left shift of uint128_t
// @param dest: destination for the left shifted uint128_t
// @param a: uint128_t to be left shifted
// @return true if the left shift is successful, false otherwise
bool assert_lshift_uint128(uint128_t *dest, uint128_t *a) {
  clear_uint128(a);
  clear_uint128(dest);

  E0(a) = 0xFFFFFFFFFFFFFFFF;
  E1(a) = 0xFFFFFFFFFFFFFFFF;

  lshift_uint128(dest, a, 1);
  if (E0(dest) == 0xFFFFFFFFFFFFFFFF && E1(dest) == 0xFFFFFFFFFFFFFFFE) {
    printf("lshift_uint128 - Passed\n");
  } else {
    printf("lshift_uint128 failed\n");
    printf("dest: ");
    print_hex_uint128(dest);
  }
}

// assert right shift of uint128_t
// @param dest: destination for the right shifted uint128_t
// @param a: uint128_t to be right shifted
// @return true if the right shift is successful, false otherwise
bool assert_rshift_uint128(uint128_t *dest, uint128_t *a) {
  clear_uint128(a);
  clear_uint128(dest);

  E0(a) = 0xFFFFFFFFFFFFFFFF;
  E1(a) = 0xFFFFFFFFFFFFFFFF;

  rshift_uint128(dest, a, 1);
  if (E0(dest) == 0x7FFFFFFFFFFFFFFF && E1(dest) == 0xFFFFFFFFFFFFFFFF) {
    printf("rshift_uint128 - Passed\n");
  } else {
    printf("rshift_uint128 failed\n");
    printf("dest: ");
    print_hex_uint128(dest);
  }
}

// generate a random uint128_t
// @param dest: the destination uint128_t
void gen_rand_uint128(uint128_t *dest) {
  E0(dest) = rand();
  E1(dest) = rand();
}

// generate a random uint256_t
// @param dest: the destination uint256_t
void gen_rand_uint256(uint256_t *dest) {
  E0(dest).e[0] = rand();
  E0(dest).e[1] = rand();
  E1(dest).e[0] = rand();
  E1(dest).e[1] = rand();
}

int main(void) {
  uint128_t a, b;
  uint256_t x, y;

  // generate random values
  srand(time(NULL));

  gen_rand_uint128(&b);
  gen_rand_uint256(&y);

  //              //
  // assertations //
  //              //

  // Copy
  assert_copy_uint128(&a, &b);
  assert_copy_uint256(&x, &y);

  printf("\n");

  // Equality
  assert_equal_uint128(&a, &b);
  assert_equal_uint256(&x, &y);
  printf("\n");

  // Clear
  assert_clear_uint128(&a);
  assert_clear_uint256(&x);

  // Shift
  // Left shift
  assert_lshift_uint128(&a, &b);

  // Right shift
  assert_rshift_uint128(&a, &b);

  return 0;
}