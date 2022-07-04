#include "bigint.h"
#include "config.h"

#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

////////////
/// INIT ///
////////////

// 128
uint128_t init_uint128(uint64_t a) {
  uint128_t result;
  E0(result) = a;
  E1(result) = a;
  return result;
}

// 256
uint256_t init_uint256(uint64_t a) {
  uint256_t result;
  E00(result) = a;
  E01(result) = a;
  E10(result) = a;
  E11(result) = a;
  return result;
}

// 512
uint512_t init_uint512(uint64_t a) {
  uint512_t result;

  change_uint512(&result, a, a, a, a, a, a, a, a);
  return result;
}

////////////////
/// INIT ALL ///
////////////////

// 128
uint128_t init_all_uint128(uint64_t a, uint64_t b) {
  uint128_t result;
  E0(result) = a;
  E1(result) = b;
  return result;
}

// 256
uint256_t init_all_uint256(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
  uint256_t result;

  E00(result) = a;
  E01(result) = b;
  E10(result) = c;
  E11(result) = d;

  return result;
}

// 512
uint512_t init_all_uint512(uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                           uint64_t e, uint64_t f, uint64_t g, uint64_t h) {
  uint512_t result;

  E0(result) = init_all_uint256(a, b, c, d);
  E1(result) = init_all_uint256(e, f, g, h);

  return result;
}

/////////////////////
/// CHANGE VALUES ///
/////////////////////

// 128
void change_uint128(uint128_t *dest, uint64_t a, uint64_t b) {
  E_0(dest) = a;
  E_1(dest) = b;
}

// 256
void change_uint256(uint256_t *dest, uint64_t a, uint64_t b, uint64_t c,
                    uint64_t d) {
  E_0_0(dest) = a;
  E_0_1(dest) = b;
  E_1_0(dest) = c;
  E_1_1(dest) = d;
}

// 512
void change_uint512(uint512_t *dest, uint64_t a, uint64_t b, uint64_t c,
                    uint64_t d, uint64_t e, uint64_t f, uint64_t g,
                    uint64_t h) {
  uint256_t temp = init_all_uint256(a, b, c, d);
  E_0(dest) = temp;
  temp = init_all_uint256(e, f, g, h);
  E_1(dest) = temp;
}

////////////
/// COPY ///
////////////

// 128
void copy_uint128(uint128_t *dest, uint128_t *src) {
  E_0(dest) = E_0(src);
  E_1(dest) = E_1(src);
}

// 256
void copy_uint256(uint256_t *dest, uint256_t *src) {
  E_0(dest) = E_0(src);
  E_1(dest) = E_1(src);
}

// 512
void copy_uint512(uint512_t *dest, uint512_t *src) {
  E_0(dest) = E_0(src);
  E_1(dest) = E_1(src);
}

//////////////////
/// CLEAR UINT ///
//////////////////

// 128
void clear_uint128(uint128_t *dest) {
  E_0(dest) = 0;
  E_1(dest) = 0;
}

// 256
void clear_uint256(uint256_t *dest) {
  clear_uint128(&E_0(dest));
  clear_uint128(&E_1(dest));
}

// 512
void clear_uint512(uint512_t *dest) {
  clear_uint256(&E_0(dest));
  clear_uint256(&E_1(dest));
}

//////////////////
/// ZERO CHECK ///
//////////////////

// 128
bool zero_uint128(uint128_t *src) { return (E_0(src) == 0 && E_1(src) == 0); }

// 256
bool zero_uint256(uint256_t *src) {
  return (zero_uint128(&E_0(src)) && zero_uint128(&E_1(src)));
}

// 512
bool zero_uint512(uint512_t *src) {
  return (zero_uint256(&E_0(src)) && zero_uint256(&E_1(src)));
}

///                                                       ///
///                       Bit stuff                       ///

///////////
/// SHL ///
///////////

// 128
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

// 256
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

// 512
void lshift_uint512(uint512_t *dest, uint512_t *src, int shift) {
  if (shift >= 512) {
    clear_uint512(dest);
  } else if (shift == 256) {
    copy_uint256(&E_0(dest), &E_1(src));
    clear_uint256(&E_1(dest));
  } else if (shift == 0) {
    copy_uint512(dest, src);
  } else if (shift < 256) {
    uint256_t tmp1;
    uint256_t tmp2;
    uint512_t result;
    lshift_uint256(&tmp1, &E_0(src), shift);
    rshift_uint256(&tmp2, &E_1(src), (256 - shift));
    add_uint256(&E0(result), &tmp1, &tmp2);
    lshift_uint256(&E1(result), &E_1(src), shift);
    copy_uint512(dest, &result);
  } else if ((512 > shift) && (shift > 256)) {
    lshift_uint256(&E_0(dest), &E_1(src), (shift - 256));
    clear_uint256(&E_1(dest));
  } else {
    clear_uint512(dest);
  }
}

/////////
// SHR //
/////////

// 128
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

// 256
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

// 512
void rshift_uint512(uint512_t *dest, uint512_t *src, int shift) {
  if (shift >= 512) {
    clear_uint512(dest);
  } else if (shift == 256) {
    copy_uint256(&E_1(dest), &E_0(src));
    clear_uint256(&E_0(dest));
  } else if (shift == 0) {
    copy_uint512(dest, src);
  } else if (shift < 256) {
    uint256_t tmp1;
    uint256_t tmp2;
    uint512_t result;

    rshift_uint256(&E0(result), &E_0(src), shift);
    rshift_uint256(&tmp1, &E_1(src), shift);
    lshift_uint256(&tmp2, &E_0(src), (256 - shift));
    add_uint256(&E1(result), &tmp1, &tmp2);
    copy_uint512(dest, &result);
  } else if ((512 > shift) && (shift > 256)) {
    rshift_uint256(&E_1(dest), &E_0(src), (shift - 256));
    clear_uint256(&E_0(dest));
  } else {
    clear_uint512(dest);
  }
}

/////////
// AND //
/////////

// 128
void and_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) & E_0(b);
  E_1(dest) = E_1(a) & E_1(b);
}

// 256
void and_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  and_uint128(&E_0(dest), &E_0(a), &E_0(b));
  and_uint128(&E_1(dest), &E_1(a), &E_1(b));
}

// 512
void and_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {
  and_uint256(&E_0(dest), &E_0(a), &E_0(b));
  and_uint256(&E_1(dest), &E_1(a), &E_1(b));
}

//////////
/// OR ///
//////////

// 128
void or_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) | E_0(b);
  E_1(dest) = E_1(a) | E_1(b);
}

// 256
void or_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  or_uint128(&E_0(dest), &E_0(a), &E_0(b));
  or_uint128(&E_1(dest), &E_1(a), &E_1(b));
}

// 512
void or_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {
  or_uint256(&E_0(dest), &E_0(a), &E_0(b));
  or_uint256(&E_1(dest), &E_1(a), &E_1(b));
}

/////////
// XOR //
/////////

// 128
void xor_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) ^ E_0(b);
  E_1(dest) = E_1(a) ^ E_1(b);
}

// 256
void xor_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  xor_uint128(&E_0(dest), &E_0(a), &E_0(b));
  xor_uint128(&E_1(dest), &E_1(a), &E_1(b));
}

// 512
void xor_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {
  xor_uint256(&E_0(dest), &E_0(a), &E_0(b));
  xor_uint256(&E_1(dest), &E_1(a), &E_1(b));
}

///////////
/// NOT ///
///////////

// 128
void not_uint128(uint128_t *dest, uint128_t *src) {
  E_0(dest) = ~E_0(src);
  E_1(dest) = ~E_1(src);
}

// 256
void not_uint256(uint256_t *dest, uint256_t *src) {
  not_uint128(&E_0(dest), &E_0(src));
  not_uint128(&E_1(dest), &E_1(src));
}

// 512
void not_uint512(uint512_t *dest, uint512_t *src) {
  not_uint256(&E_0(dest), &E_0(src));
  not_uint256(&E_1(dest), &E_1(src));
}

//////////
/// LT ///
//////////

// 128
bool lt_uint128(uint128_t *a, uint128_t *b) {
  if (E_0(a) == E_0(b)) {
    return E_1(a) < E_1(b);
  }
  return (E_0(a) < E_0(b));
}

// 256
bool lt_uint256(uint256_t *a, uint256_t *b) {
  if (equal_uint128(&E_0(a), &E_0(b))) {
    return lt_uint128(&E_1(a), &E_1(b));
  }
  return lt_uint128(&E_0(a), &E_0(b));
}

// 512
bool lt_uint512(uint512_t *a, uint512_t *b) {
  if (equal_uint256(&E_0(a), &E_0(b))) {
    return lt_uint256(&E_1(a), &E_1(b));
  }
  return lt_uint256(&E_0(a), &E_0(b));
}

//////////
/// GT ///
//////////

// 128
bool gt_uint128(uint128_t *a, uint128_t *b) {
  if (E_0(a) == E_0(b)) {
    return E_1(a) > E_1(b);
  }
  return (E_0(a) > E_0(b));
}

// 256
bool gt_uint256(uint256_t *a, uint256_t *b) {
  if (equal_uint128(&E_0(a), &E_0(b))) {
    return gt_uint128(&E_1(a), &E_1(b));
  }
  return gt_uint128(&E_0(a), &E_0(b));
}

// 512
bool gt_uint512(uint512_t *a, uint512_t *b) {
  if (equal_uint256(&E_0(a), &E_0(b))) {
    return gt_uint256(&E_1(a), &E_1(b));
  }
  return gt_uint256(&E_0(a), &E_0(b));
}

///////////
/// LTE ///
///////////

// 128
bool lte_uint128(uint128_t *a, uint128_t *b) {
  return lt_uint128(a, b) || equal_uint128(a, b);
}

// 256
bool lte_uint256(uint256_t *a, uint256_t *b) {
  return lt_uint256(a, b) || equal_uint256(a, b);
}

// 512
bool lte_uint512(uint512_t *a, uint512_t *b) {
  return lt_uint512(a, b) || equal_uint512(a, b);
}

/////////
// GTE //
/////////

// 128
bool gte_uint128(uint128_t *a, uint128_t *b) {
  return gt_uint128(a, b) || equal_uint128(a, b);
}

// 256
bool gte_uint256(uint256_t *a, uint256_t *b) {
  return gt_uint256(a, b) || equal_uint256(a, b);
}

// 512
bool gte_uint512(uint512_t *a, uint512_t *b) {
  return gt_uint512(a, b) || equal_uint512(a, b);
}

///                                                        ///
///                       Arithmetic                       ///

/////////
// ADD //
/////////

// 128
void add_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) + E_0(b) + ((E_1(a) + E_1(b)) < E_1(a));
  E_1(dest) = E_1(a) + E_1(b);
}

// 256
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

// 512
void add_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {
  uint256_t tmp;
  add_uint256(&E_0(dest), &E_0(a), &E_0(b));
  add_uint256(&tmp, &E_1(a), &E_1(b));
  if (gt_uint256(&E_1(a), &tmp)) {
    uint256_t one;
    E0(one) = init_uint128(0);
    E1(one) = init_all_uint128(0, 1);

    add_uint256(&E_0(dest), &E_0(dest), &one);
  } else {
    add_uint256(&E_1(dest), &E_1(a), &E_1(b));
  }
}

///////////
/// SUB ///
///////////

// 128
void sub_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  E_0(dest) = E_0(a) - E_0(b) - ((E_1(a) - E_1(b)) > E_1(a));
  E_1(dest) = E_1(a) - E_1(b);
}

// 256
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

// 512
void sub_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {
  uint256_t tmp;

  sub_uint256(&E_0(dest), &E_0(a), &E_0(b));
  sub_uint256(&tmp, &E_1(a), &E_1(b));
  if (gt_uint256(&tmp, &E_1(a))) {
    uint256_t one;
    E0(one) = init_uint128(0);
    E1(one) = init_all_uint128(0, 1);

    sub_uint256(&E_0(dest), &E_0(dest), &one);
  } else {
    sub_uint256(&E_1(dest), &E_1(a), &E_1(b));
  }
}

///////////
/// MUL ///
///////////

// 128
void mul_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {
  uint64_t top[4] = {E_0(a) >> 32, E_0(a) & 0xffffffff, E_1(a) >> 32,
                     E_1(a) & 0xffffffff};
  uint64_t bottom[4] = {E_0(b) >> 32, E_0(b) & 0xffffffff, E_1(b) >> 32,
                        E_1(b) & 0xffffffff};
  uint64_t products[4][4];
  uint128_t tmp, tmp2;

  for (int y = 3; y > -1; y--) {
    for (int x = 3; x > -1; x--) {
      products[3 - x][y] = top[x] * bottom[y];
    }
  }

  uint64_t fourth32 = products[0][3] & 0xffffffff;
  uint64_t third32 = (products[0][2] & 0xffffffff) + (products[0][3] >> 32);
  uint64_t second32 = (products[0][1] & 0xffffffff) + (products[0][2] >> 32);
  uint64_t first32 = (products[0][0] & 0xffffffff) + (products[0][1] >> 32);

  third32 += products[1][3] & 0xffffffff;
  second32 += (products[1][2] & 0xffffffff) + (products[1][3] >> 32);
  first32 += (products[1][1] & 0xffffffff) + (products[1][2] >> 32);

  second32 += products[2][3] & 0xffffffff;
  first32 += (products[2][2] & 0xffffffff) + (products[2][3] >> 32);

  first32 += products[3][3] & 0xffffffff;

  E0(tmp) = first32 << 32;
  E1(tmp) = 0;
  E0(tmp2) = third32 >> 32;
  E1(tmp2) = third32 << 32;
  add_uint128(dest, &tmp, &tmp2);
  E0(tmp) = second32;
  E1(tmp) = 0;
  add_uint128(&tmp2, &tmp, dest);
  E0(tmp) = 0;
  E1(tmp) = fourth32;
  add_uint128(dest, &tmp, &tmp2);
}

// 256
void mul_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {
  uint128_t top[4];
  uint128_t bottom[4];
  uint128_t products[4][4];
  uint128_t tmp, tmp2, fourth64, third64, second64, first64;
  uint256_t target1, target2;
  E0(top[0]) = 0;
  E1(top[0]) = E0(E_0(a));
  E0(top[1]) = 0;
  E1(top[1]) = E1(E_0(a));
  E0(top[2]) = 0;
  E1(top[2]) = E0(E_1(a));
  E0(top[3]) = 0;
  E1(top[3]) = E1(E_1(a));
  E0(bottom[0]) = 0;
  E1(bottom[0]) = E0(E_0(b));
  E0(bottom[1]) = 0;
  E1(bottom[1]) = E1(E_0(b));
  E0(bottom[2]) = 0;
  E1(bottom[2]) = E0(E_1(b));
  E0(bottom[3]) = 0;
  E1(bottom[3]) = E1(E_1(b));

  int y;

  for (y = 3; y > -1; --y) {
    for (int x = 3; x > -1; --x) {
      mul_uint128(&products[3 - x][y], &top[x], &bottom[y]);
    }
  }

  E0(fourth64) = 0;
  E1(fourth64) = E1(products[0][3]);
  E0(tmp) = 0;
  E1(tmp) = E1(products[0][2]);
  E0(tmp2) = 0;
  E1(tmp2) = E0(products[0][3]);
  add_uint128(&third64, &tmp, &tmp2);
  E0(tmp) = 0;
  E1(tmp) = E1(products[0][1]);
  E0(tmp2) = 0;
  E1(tmp2) = E0(products[0][2]);
  add_uint128(&second64, &tmp, &tmp2);
  E0(tmp) = 0;
  E1(tmp) = E1(products[0][0]);
  E0(tmp2) = 0;
  E1(tmp2) = E0(products[0][1]);
  add_uint128(&first64, &tmp, &tmp2);

  E0(tmp) = 0;
  E1(tmp) = E1(products[1][3]);
  add_uint128(&tmp2, &tmp, &third64);
  copy_uint128(&third64, &tmp2);
  E0(tmp) = 0;
  E1(tmp) = E1(products[1][2]);
  add_uint128(&tmp2, &tmp, &second64);
  E0(tmp) = 0;
  E1(tmp) = E0(products[1][3]);
  add_uint128(&second64, &tmp, &tmp2);
  E0(tmp) = 0;
  E1(tmp) = E1(products[1][1]);
  add_uint128(&tmp2, &tmp, &first64);
  E0(tmp) = 0;
  E1(tmp) = E0(products[1][2]);
  add_uint128(&first64, &tmp, &tmp2);

  E0(tmp) = 0;
  E1(tmp) = E1(products[2][3]);
  add_uint128(&tmp2, &tmp, &second64);
  copy_uint128(&second64, &tmp2);
  E0(tmp) = 0;
  E1(tmp) = E1(products[2][2]);
  add_uint128(&tmp2, &tmp, &first64);
  E0(tmp) = 0;
  E1(tmp) = E0(products[2][3]);
  add_uint128(&first64, &tmp, &tmp2);

  E0(tmp) = 0;
  E1(tmp) = E1(products[3][3]);
  add_uint128(&tmp2, &tmp, &first64);
  copy_uint128(&first64, &tmp2);

  clear_uint256(&target1);
  lshift_uint128(&E0(target1), &first64, 64);
  clear_uint256(&target2);
  E0(E0(target2)) = E0(third64);
  lshift_uint128(&E1(target2), &third64, 64);
  add_uint256(dest, &target1, &target2);
  clear_uint256(&target1);
  copy_uint128(&E0(target1), &second64);
  add_uint256(&target2, &target1, dest);
  clear_uint256(&target1);
  copy_uint128(&E1(target1), &fourth64);
  add_uint256(dest, &target1, &target2);
}

// 512
void mul_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {}
///////////
/// DIV ///
///////////

// 128
void div_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {}

// 256
void div_uint256(uint256_t *dest, uint256_t *a, uint256_t *b) {}

// 512
void div_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {}

//////////////
/// DIVMOD ///
//////////////

// 128
void divmod_uint128(uint128_t *retDiv, uint128_t *retMod, uint128_t *l,
                    uint128_t *r) {}

// 256

// 512

///////////
/// EXP ///
///////////

// 128
void exp_uint128(uint128_t *dest, uint128_t *a, uint128_t *b) {}

// 256
// 512
// 512
void exp_uint512(uint512_t *dest, uint512_t *a, uint512_t *b) {}

/////////////
/// EQUAL ///
/////////////

// 128
bool equal_uint128(uint128_t *a, uint128_t *b) {
  return E_0(a) == E_0(b) && E_1(a) == E_1(b);
}

// 256
bool equal_uint256(uint256_t *a, uint256_t *b) {
  return E_0_0(a) == E_0_0(b) && E_0_1(a) == E_0_1(b) && E_1_0(a) == E_1_0(b) &&
         E_1_1(a) == E_1_1(b);
}

// 512
bool equal_uint512(uint512_t *a, uint512_t *b) {
  return equal_uint256(&E_0(a), &E_0(b)) && equal_uint256(&E_1(a), &E_1(b));
}

///                                                          ///
///                           Misc                           ///

////////////////
/// PRINTING ///
////////////////

// 128
void print_hex_uint128(uint128_t *a) {
  printf("%016llX%016llX", E_0(a), E_1(a));
}

// 256
void print_hex_uint256(uint256_t *a) {
  printf("%016llX%016llX%016llX%016llX", E_0(a), E_1(a));
}

// 512
void print_hex_uint512(uint512_t *a) {
  printf("%016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX", E_0(a),
         E_1(a));
}

//////////////////
/// HEX LENGTH ///
//////////////////

// <=64
int hex_length(uint64_t *src) {
  // get length of src in base 16
  int x = ceil(log(*src + 1) / log(16));
  // if length is odd, add one else return x
  // accounts for bytes that start at 0 (e.g. 01...0F)
  x % 2 ? x += 1 : x;
  // only care about whole bytes, not individual digits
  return x / 2;
}

// 128
int hex_length_uint128(uint128_t *src) {
  int x = hex_length(&E_0(src));
  // if the first uint64 is 0, then return the length of the second uint128
  // else add the length of the first uint128 & add 8
  return x == 0 ? hex_length(&E_1(src)) : x + 8;
}

// 256
int hex_length_uint256(uint256_t *src) {
  int x = hex_length_uint128(&E_0(src));
  return x == 0 ? hex_length_uint128(&E_1(src)) : x + 16;
}

// 512
int hex_length_uint512(uint512_t *src) {
  int x = hex_length_uint256(&E_0(src));
  return x == 0 ? hex_length_uint256(&E_1(src)) : x + 32;
}

////////////////
/// GET UINT ///
////////////////

// 256
uint64_t get_uint64(uint256_t *src, int index) {
  switch (index) {
  case 0:
    return E_0_0(src);
    break;
  case 1:
    return E_0_1(src);
    break;
  case 2:
    return E_1_0(src);
    break;
  case 3:
    return E_1_1(src);
    break;
  default:
    return 0;
    break;
  }
}