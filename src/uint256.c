#include "uint256.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

uint128_t init_all_uint128(uint64_t a, uint64_t b) {
  uint128_t result;
  E0(result) = a;
  E1(result) = b;
  return result;
}

uint256_t init_all_uint256(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
  uint256_t result;

  E00(result) = a;
  E01(result) = b;
  E10(result) = c;
  E11(result) = d;

  return result;
}

uint128_t init_uint128(uint64_t a) {
  uint128_t result;
  E0(result) = a;
  E1(result) = a;
  return result;
}

uint256_t init_uint256(uint64_t a) {
  uint256_t result;
  E00(result) = a;
  E01(result) = a;
  E10(result) = a;
  E11(result) = a;
  return result;
}

void change_uint128(uint128_t *dest, uint64_t a, uint64_t b) {
  E_0(dest) = a;
  E_1(dest) = b;
}

void change_uint256( uint256_t *dest, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {  
  E_0_0(dest) = a;
  E_0_1(dest) = b;
  E_1_0(dest) = c;
  E_1_1(dest) = d;
}

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
  return E_0_0(a) == E_0_0(b) && E_0_1(a) == E_0_1(b) && E_1_0(a) == E_1_0(b) &&
         E_1_1(a) == E_1_1(b);
}

bool lt_uint128(uint128_t *a, uint128_t *b) {
  if (E_0(a) == E_0(b)) {
    return E_1(a) < E_1(b);
  }
  return (E_0(a) < E_0(b));
}

bool lt_uint256(uint256_t *a, uint256_t *b) {
  if (equal_uint128(&E_0(a), &E_0(b))) {
    return lt_uint128(&E_1(a), &E_1(b));
  }
  return lt_uint128(&E_0(a), &E_0(b));
}

bool gt_uint128(uint128_t *a, uint128_t *b) {
  if (E_0(a) == E_0(b)) {
    return E_1(a) > E_1(b);
  }
  return (E_0(a) > E_0(b));
}

bool gt_uint256(uint256_t *a, uint256_t *b) {
  if (equal_uint128(&E_0(a), &E_0(b))) {
    return gt_uint128(&E_1(a), &E_1(b));
  }
  return gt_uint128(&E_0(a), &E_0(b));
}

bool gte(uint128_t *a, uint128_t *b) {
  return gt_uint128(a, b) || equal_uint128(a, b);
}

bool gte_uint256(uint256_t *a, uint256_t *b) {
  return gt_uint256(a, b) || equal_uint256(a, b);
}

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
    rshift_uint128(&tmp2, &E_1(src), (128 - shift));
    add_uint128(&E0(result), &tmp1, &tmp2);
    lshift_uint128(&E1(result), &E_1(src), shift);
    copy_uint256(dest, &result);
  } else if ((256 > shift) && (shift > 128)) {
    lshift_uint128(&E_0(dest), &E_1(src), (shift - 128));
    clear_uint128(&E_1(dest));
  } else {
    clear_uint256(dest);
  }
}

void rshift_uint128(uint128_t *dest, uint128_t *src, int shift) {

  if (shift >= 128) {
    clear_uint128(dest);
  } else if (shift == 64) {
    E_0(dest) = 0;
    E_1(dest) = E_0(src);

  } else if (shift == 0) {
    copy_uint128(dest, src);
  } else if (shift < 64) {

    E_0(dest) = E_0(src) >> shift;

    E_1(dest) = (E_0(src) << (64 - shift)) + (E_1(src) >> shift);

  } else if ((128 > shift) && (shift > 64)) {

    E_0(dest) = 0;
    E_1(dest) = E_0(src) >> (shift - 64);
  } else {
    clear_uint128(dest);
  }
}

void rshift_uint256(uint256_t *dest, uint256_t *src, int shift) {
  if (shift >= 256) {
    clear_uint256(dest);
  } else if (shift == 128) {
    copy_uint128(&E_1(dest), &E_0(src));
    clear_uint128(&E_0(dest));
  } else if (shift == 0) {
    copy_uint256(dest, src);
  } else if (shift < 128) {
    uint128_t tmp1;
    uint128_t tmp2;
    uint256_t result;
    rshift_uint128(&E0(result), &E_0(src), shift);
    rshift_uint128(&tmp1, &E_1(src), shift);
    lshift_uint128(&tmp2, &E_0(src), (128 - shift));
    add_uint128(&E1(result), &tmp1, &tmp2);
    copy_uint256(dest, &result);
  } else if ((256 > shift) && (shift > 128)) {
    rshift_uint128(&E_1(dest), &E_0(src), (shift - 128));
    clear_uint128(&E_0(dest));
  } else {
    clear_uint256(dest);
  }
}

void add_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) + E_0(b) + ((E_1(a) + E_1(b)) < E_1(a));
  E_1(dest) = E_1(a) + E_1(b);
}

void add_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  uint128_t tmp;
  add_uint128(&E_0(dest), &E_0(a), &E_0(b));
  add_uint128(&tmp, &E_1(a), &E_1(b));
  if (gt_uint128(&E_1(a), &tmp)) {
    uint128_t one;
    E0(one) = 0;
    E1(one) = 1;

    add_uint128(&E_0(dest), &E_0(dest), &one);
  } else {
    add_uint128(&E_1(dest), &E_1(a), &E_1(b));
  }
}

void sub_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) - E_0(b) - ((E_1(a) - E_1(b)) > E_1(a));
  E_1(dest) = E_1(a) - E_1(b);
}

void sub_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  uint128_t tmp;

  sub_uint128(&E_0(dest), &E_0(a), &E_0(b));
  sub_uint128(&tmp, &E_1(a), &E_1(b));
  if (gt_uint128(&tmp, &E_1(a))) {
    uint128_t one;
    E0(one) = 0;
    E1(one) = 1;

    sub_uint128(&E_0(dest), &E_0(dest), &one);
  } else {
    sub_uint128(&E_1(dest), &E_1(a), &E_1(b));
  }
}

void and_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) & E_0(b);
  E_1(dest) = E_1(a) & E_1(b);
}

void and_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  E_0_0(dest) = E_0_0(a) & E_0_0(b);
  E_0_1(dest) = E_0_1(a) & E_0_1(b);
  E_1_0(dest) = E_1_0(a) & E_1_0(b);
  E_1_1(dest) = E_1_1(a) & E_1_1(b);
}
void xor_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {}

void xor_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {}

void or_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) | E_0(b);
  E_1(dest) = E_1(a) | E_1(b);
}
void or_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  E_0_0(dest) = E_0_0(a) | E_0_0(b);
  E_0_1(dest) = E_0_1(a) | E_0_1(b);
  E_1_0(dest) = E_1_0(a) | E_1_0(b);
  E_1_1(dest) = E_1_1(a) | E_1_1(b);
}

// printf functions //

void print_hex_uint128(uint128_t *a) {
  printf("%016llX%016llX", E_0(a), E_1(a));
}

void print_hex_uint256(uint256_t *a) {
  printf("%016llX%016llX%016llX%016llX", E_0(a), E_1(a));
}