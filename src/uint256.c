#include "uint256.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

uint128_t *create_uint128(void) {
  uint128_t *result;
  clear_uint128(result);
  return result;
}

uint256_t *create_uint256(void) {
  uint256_t *result;
  clear_uint256(result);
  return result;
}

uint128_t *set_uint128(uint64_t a, uint64_t b) {
  uint128_t *result;
  clear_uint128(result);
  E_0(result) = a;
  E_1(result) = b;
  return result;
}

uint256_t *set_uint256(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
  uint256_t *result;
  clear_uint256(result);
  E_0(result) = *set_uint128(a, b);
  E_1(result) = *set_uint128(c, d);

  return result;
}

// all functions before this point are broken

void copy_uint128(uint128_t *dest, uint128_t *src) {
  E_0(dest) = E_0(src);
  E_1(dest) = E_1(src);
}

void copy_uint256(uint256_t *dest, uint256_t *src) {
  E_0(dest) = E_0(src);
  E_1(dest) = E_1(src);
}

void clear_uint128(uint128_t *dest) {
  E_0(dest) = 0;
  E_1(dest) = 0;
}

void clear_uint256(uint256_t *dest) {
  uint128_t x;
  clear_uint128(&x);
  E_0(dest) = x;
  E_1(dest) = x;
}

bool equal_uint128(uint128_t *a, uint128_t *b) {
  return E_0(a) == E_0(b) && E_1(a) == E_1(b);
}

bool equal_uint256(uint256_t *a, uint256_t *b) {
  return E_0(a).e[0] == E_0(b).e[0] && E_0(a).e[1] == E_0(b).e[1];
}

// UNDER CONSTRUCTION - TEST PLZ
void lshift_uint128(uint128_t *dest, uint128_t *src, int shift) {
  if (shift >= 128) {
    clear_uint128(dest);
  } else if (shift == 64) {
    E_0(dest) = E_1(src);
    E_1(dest) = 0;

  } else if (shift == 0) {
    copy_uint128(dest, src);

  } else if (shift < 64) {

    E_0(dest) = (E_0(src) << shift) + (E_1(src) >> (64 - shift));
    E_1(dest) = (E_1(src) << shift);

  } else if ((128 > shift) && (shift > 64)) {
    E_0(dest) = E_1(src) << (shift - 64);
    E_1(dest) = 0;
  } else {
    clear_uint128(dest);
  }
}

void lshift_uint256(uint256_t *dest, uint256_t *src, int shift) {
  if (shift >= 256) {
    clear_uint256(dest);
  } else if (shift == 128) {
    copy_uint128(&E_0(dest), &E_1(src));
    clear_uint128(&E_1(dest));
  } else if (shift == 0) {
    copy_uint256(dest, src);
  } else if (shift < 128) {
    uint128_t tmp1;
    uint128_t tmp2;
    uint256_t result;
    lshift_uint128(&tmp1, &E_0(src), shift);
    rshift_uint128(&tmp2, &E_1(src), shift);
    add_uint128(&E1(result), &tmp1, &tmp2);
    lshift_uint128(&E1(result), &E_1(src), shift);
  } else if ((256 > shift) && (shift > 128)) {

  } else {
    clear_uint256(dest);
  }
}

void rshift_uint128(uint128_t *dest, uint128_t *src, int shift) {

  if (shift >= 128) {
    clear_uint128(dest);
  } else if (shift == 64) {
    E_0(dest) = 0;
    E_1(dest) = E_1(src);

  } else if (shift == 0) {
    copy_uint128(dest, src);
  } else if (shift < 64) {
    E_0(dest) = (E_0(src) >> shift);
    E_1(dest) = (E_1(src) >> shift) + (E_0(src) << (64 - shift));

  } else if ((128 > shift) && (shift > 64)) {
    E_0(dest) = 0;
    E_1(dest) = E_0(src) >> (shift - 64);

  } else {
    clear_uint128(dest);
  }
}

void rshift_uint256(uint256_t *dest, uint256_t *src, int shift) {}

void add_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) + E_0(b) + (E_1(a) + E_1(b) < E_1(a));
  E_1(dest) = E_1(a) + E_1(b);
}

void add_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {}

void sub_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) - E_0(b) - (E_1(a) - E_1(b) > E_1(a));
  E_1(dest) = E_1(a) - E_1(b);
}

void sub_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {}

void and_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {}

void and_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {}
void xor_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {}

void xor_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {}

// printf functions

void print_hex_uint128(uint128_t *a) {
  printf("%016llX%016llX\n", E_0(a), E_1(a));
}

void print_hex_uint256(uint256_t *a) {
  printf("%016llX%016llX%016llX%016llX\n", E_0(a), E_1(a));
}