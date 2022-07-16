#ifndef BIG_INT_H
#define BIG_INT_H

#include <inttypes.h>
#include <stdbool.h>

/*
  ┌───────────────────────────────┐
  │   UINT TYPE DEFINITIONS       │
  └───────────────────────────────┘
 */

// uint128_t by packing 2 uint64_ts into a single struct
typedef struct uint128_t {
  uint64_t e[2]; // element
} uint128_t;

// uint256_t by packing 2 uint128_ts into a single struct
typedef struct uint256_t {
  uint128_t e[2]; // element
} uint256_t;

typedef struct uint512_t {
  uint256_t e[2]; // element
} uint512_t;

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   MACROS                                                                   │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   MAX VALS OF EACH UINT TYPE  │
  └───────────────────────────────┘
 */

#define UINT128_MAX ((uint128_t){.e = {UINT64_MAX, UINT64_MAX}})
#define UINT256_MAX ((uint256_t){.e = {UINT128_MAX, UINT128_MAX}})
#define UINT512_MAX ((uint512_t){.e = {UINT256_MAX, UINT256_MAX}})

/*
  ┌───────────────────────────────┐
  │   ELEMENT ACCESSING MACROS    │
  └───────────────────────────────┘
 */

//    accessing uint64_t elements in uint128_t    //
// or accessing uint128_t elements in a uint256_t //

#define E_0(x) x->e[0]
#define E_1(x) x->e[1]

#define E0(x) x.e[0]
#define E1(x) x.e[1]

// accessing uint64_t elements in a uint256_t //

#define E_0_0(x) x->e[0].e[0]
#define E_0_1(x) x->e[0].e[1]
#define E_1_0(x) x->e[1].e[0]
#define E_1_1(x) x->e[1].e[1]

#define E00(x) x.e[0].e[0]
#define E01(x) x.e[0].e[1]
#define E10(x) x.e[1].e[0]
#define E11(x) x.e[1].e[1]

// accessing uint64_t elements in a uint512_t //

#define E_0_0_0(x) x->e[0].e[0].e[0]
#define E_0_0_1(x) x->e[0].e[0].e[1]
#define E_0_1_0(x) x->e[0].e[1].e[0]
#define E_0_1_1(x) x->e[0].e[1].e[1]
#define E_1_0_0(x) x->e[1].e[0].e[0]
#define E_1_0_1(x) x->e[1].e[0].e[1]
#define E_1_1_0(x) x->e[1].e[1].e[0]
#define E_1_1_1(x) x->e[1].e[1].e[1]

#define E000(x) x.e[0].e[0].e[0]
#define E001(x) x.e[0].e[0].e[1]
#define E010(x) x.e[0].e[1].e[0]
#define E011(x) x.e[0].e[1].e[1]
#define E100(x) x.e[1].e[0].e[0]
#define E101(x) x.e[1].e[0].e[1]
#define E110(x) x.e[1].e[1].e[0]
#define E111(x) x.e[1].e[1].e[1]

#define DEC_UINT128 "%020lld%020lld"
#define DEC_UINT256 "%020lld%020lld%020llX%020lld"
#define DEC_UINT512 "%020lld%020lld%020llX%020lld%020lld%020lld%020llX%020lld"

#define HEX_UINT128 "%016llX%016llX"
#define HEX_UINT256 "%016llX%016llX%016llX%016llX"
#define HEX_UINT512 "%016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX"

#define UINT_128(x) (x)->e[0], (x)->e[1]
#define UINT_256(x) E_0_0(x), E_0_1(x), E_1_0(x), E_1_1(x)
#define UINT_512(x) E_0_0_0(x), E_0_0_1(x), E_0_1_0(x), E_0_1_1(x), E_1_0_0(x), E_1_0_1(x), E_1_1_0(x), E_1_1_1(x)

#define UINT128(x) (x).e[0], (x).e[1]
#define UINT256(x) UINT128((x).e[0]), UINT128((x).e[1])
#define UINT512(x) UINT256((x).e[0]), UINT256((x).e[1])

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   FUNCTIONS                                                                │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   INIT                        │
  └───────────────────────────────┘
 */

// create a uint128_t initialized with a uint64_t
// @param: the uint64_t to initialize each uint64_t element of the uint128_t to
// @return the newly created uint128_t
uint128_t init_uint128(uint64_t a);
// create a uint256_t initialized with uint64_t
// @param: the uint64_t to initialize each uint64_t element of the uint256_t to
// @return the initted uint256_t
uint256_t init_uint256(uint64_t a);

// create a uint512_t initialized with uint64_t
// @param: the uint64_t to initialize each uint64_t element of the uint512_t to
// @return the initted uint512_t
uint512_t init_uint512(uint64_t a);

/*
  ┌───────────────────────────────┐
  │   INIT ALL VALUES             │
  └───────────────────────────────┘
 */

// set elements in a uint128_t
// @param a: the 1st element to change (uint128_t.uint64_t[0])
// @param b: the 2nd element to change (uint128_t.uint64_t[1])
// @return the newly created uint128_t
uint128_t init_all_uint128(uint64_t a, uint64_t b);

// set elements in a uint256_t
// @param a: the 1st element to set (uint256_t.uint128_t[0].uint64_t[0])
// @param b: the 2nd element to set (uint256_t.uint128_t[0].uint64_t[1])
// @param c: the 3rd element to set (uint256_t.uint128_t[1].uint64_t[0])
// @param d: the 4th element to set (uint256_t.uint128_t[1].uint64_t[1])
// @return a newly created uint256_t
uint256_t init_all_uint256(uint64_t a, uint64_t b, uint64_t c, uint64_t d);

// checkout init_all_uint256 && init_all_uint128 for reference
uint512_t init_all_uint512(uint64_t a, uint64_t b, uint64_t c, uint64_t d,
                           uint64_t e, uint64_t f, uint64_t g, uint64_t h);

/*
  ┌───────────────────────────────┐
  │   CHANGE INDEX VALUE          │
  └───────────────────────────────┘
 */

// change an element in a uint128_t
// @param dest: the uint128_t to change
// @param index: the index of the element to change (0 or 1)
// @param value: the value to change the element to
void change_uint128(uint128_t *dest, int index, uint64_t a);

// change an element in a uint256_t
// @param dest: the uint256_t to change
// @param index: the index of the element to change (0 to 3)
// @param value: the value to change the element to
void change_uint256(uint256_t *dest, int index, uint64_t a);

// change an element in a uint512_t
// @param dest: the uint512_t to change
// @param index: the index of the element to change (0 to 7)
// @param value: the value to change the element to
void change_uint512(uint512_t *dest, int index, uint64_t a);

/*
  ┌───────────────────────────────┐
  │   CHANGE ALL VALUES           │
  └───────────────────────────────┘
 */

// change elements in an already initialized uint128_t
// @param dest: the uint128_t to change
// @param a: the 1st element to change (uint128_t.uint64_t[0])
// @param b: the 2nd element to change (uint128_t.uint64_t[1])
void change_all_uint128(uint128_t *dest, uint64_t a, uint64_t b);

// change elements in an already initialized uint256_t
// @param dest: the uint256_t to change
// @param a: the 1st element to change (uint256_t.uint128_t[0].uint64_t[0])
// @param b: the 2nd element to change (uint256_t.uint128_t[0].uint64_t[1])
// @param c: the 3rd element to change (uint256_t.uint128_t[1].uint64_t[0])
// @param d: the 4th element to change (uint256_t.uint128_t[1].uint64_t[1])
void change_all_uint256(uint256_t *dest, uint64_t a, uint64_t b, uint64_t c,
                        uint64_t d);

// check change_all_uint256 && change_all_uint128 for reference
void change_all_uint512(uint512_t *dest, uint64_t a, uint64_t b, uint64_t c,
                        uint64_t d, uint64_t e, uint64_t f, uint64_t g,
                        uint64_t h);

/*
  ┌───────────────────────────────┐
  │   MAX VALUE                   │
  └───────────────────────────────┘
 */

// get max value of a uint128_t
// @return the max value of a uint128_t
uint128_t max_uint128(void);

// get max value of a uint256_t
// @return the max value of a uint256_t
uint256_t max_uint256(void);

// get max value of a uint512_t
// @return the max value of a uint512_t
uint512_t max_uint512(void);

/*
  ┌───────────────────────────────┐
  │   COPY                        │
  └───────────────────────────────┘
 */

// copy a uint128_t into another uint128_t
// @param dest: the destination uint128_t to copy src into
// @param src: the source uint128_t to copy into dest
void copy_uint128(uint128_t *dest, uint128_t *src);

// copy a uint256_t into another uint256_t
// @param dest: the destination uint256_t to copy src into
// @param src: the source uint256_t to copy into dest
void copy_uint256(uint256_t *dest, uint256_t *src);

// copy a uint512_t into another uint512_t
// @param dest: the destination uint512_t to copy src into
// @param src: the source uint512_t to copy into dest
void copy_uint512(uint512_t *dest, uint512_t *src);

/*
  ┌───────────────────────────────┐
  │   CLEAR UINT                  │
  └───────────────────────────────┘
 */

// set a uint128_t to 0
// @param dest: the uint128_t to set to 0
void clear_uint128(uint128_t *dest);

// set a uint256_t to 0
// @param dest: the uint256_t to set to 0
void clear_uint256(uint256_t *dest);

// set a uint512_t to 0
// @param dest: the uint512_t to set to 0
void clear_uint512(uint512_t *dest);

/*
  ┌───────────────────────────────┐
  │   ZERO CHECK                  │
  └───────────────────────────────┘
 */

// check if a uint128_t is 0
// @param src: the uint128_t to check
// @return true if the uint128_t is 0, false otherwise
bool zero_uint128(uint128_t *src);

// check if a uint256_t is 0
// @param src: the uint256_t to check
// @return true if the uint256_t is 0, false otherwise
bool zero_uint256(uint256_t *src);

// check if a uint512_t is 0
// @param src: the uint512_t to check
// @return true if the uint512_t is 0, false otherwise
bool zero_uint512(uint512_t *src);

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   BIT STUFF                                                                │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   SHL                         │
  └───────────────────────────────┘
 */

// shift a uint128_t left by n bits
// @param dest: where to store the shifted uint128_t
// @param src: the uint128_t to shift
// @param shift: the number of bits to shift
void lshift_uint128(uint128_t *dest, uint128_t *src, int shift);

// shift a uint256_t left by n bits
// @param dest: where to store the shifted uint256_t
// @param src: the uint256_t to shift
// @param shift: the number of bits to shift
void lshift_uint256(uint256_t *dest, uint256_t *src, int shift);

// shift a uint512_t left by n bits
// @param dest: where to store the shifted uint512_t
// @param src: the uint512_t to shift
// @param shift: the number of bits to shift
void lshift_uint512(uint512_t *dest, uint512_t *src, int shift);

/*
  ┌───────────────────────────────┐
  │   SHR                         │
  └───────────────────────────────┘
 */

// shift a uint128_t right by n bits
// @param dest: where to store the shifted uint128_t
// @param src: the uint128_t to shift
// @param shift: the number of bits to shift
void rshift_uint128(uint128_t *dest, uint128_t *src, int shift);

// shift a uint256_t right by n bits
// @param dest: where to store the shifted uint256_t
// @param src: the uint256_t to shift
// @param shift: the number of bits to shift
void rshift_uint256(uint256_t *dest, uint256_t *src, int shift);

// shift a uint512_t right by n bits
// @param dest: where to store the shifted uint512_t
// @param src: the uint512_t to shift
// @param shift: the number of bits to shift
void rshift_uint512(uint512_t *dest, uint512_t *src, int shift);

/*
  ┌───────────────────────────────┐
  │   AND                         │
  └───────────────────────────────┘
 */

// AND two uint128_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint128_t to AND
// @param b: the second uint128_t to AND
void and_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// AND two uint256_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint256_t to AND
// @param b: the second uint256_t to AND
void and_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// AND two uint512_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint512_t to AND
// @param b: the second uint512_t to AND
void and_uint512(uint512_t *dest, uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   OR                          │
  └───────────────────────────────┘
 */

// OR two uint128_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint128_t to OR
// @param b: the second uint128_t to OR
void or_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// OR two uint256_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint256_t to OR
// @param b: the second uint256_t to OR
void or_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// OR two uint512_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint512_t to OR
// @param b: the second uint512_t to OR
void or_uint512(uint512_t *dest, uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   XOR                         │
  └───────────────────────────────┘
 */

// XOR two uint128_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint128_t to XOR
// @param b: the second uint128_t to XOR
void xor_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// XOR two uint256_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint256_t to XOR
// @param b: the second uint256_t to XOR
void xor_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// XOR two uint512_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint512_t to XOR
// @param b: the second uint512_t to XOR
void xor_uint512(uint512_t *dest, uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   NOT                         │
  └───────────────────────────────┘
 */

// NOT a uint128_t
// @param dest: where to store the result
// @param src: the uint128_t to NOT
void not_uint128(uint128_t *dest, uint128_t *src);

// NOT a uint256_t
// @param dest: where to store the result
// @param src: the uint256_t to NOT
void not_uint256(uint256_t *dest, uint256_t *src);

// NOT a uint512_t
// @param dest: where to store the result
// @param src: the uint512_t to NOT
void not_uint512(uint512_t *dest, uint512_t *src);

/*
  ┌───────────────────────────────┐
  │   LT                          │
  └───────────────────────────────┘
 */

// less than comparison for uint128_ts
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a < b, false otherwise
bool lt_uint128(uint128_t *a, uint128_t *b);

// less than comparison for uint256_ts
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return true if a < b, false otherwise
bool lt_uint256(uint256_t *a, uint256_t *b);

// less than comparison for uint512_ts
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return true if a < b, false otherwise
bool lt_uint512(uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   GT                          │
  └───────────────────────────────┘
 */

// greater than comparison for uint128_ts
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a > b, false otherwise
bool gt_uint128(uint128_t *a, uint128_t *b);

// greater than comparison for uint256_ts
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return true if a > b, false otherwise
bool gt_uint256(uint256_t *a, uint256_t *b);

// greater than comparison for uint512_ts
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return true if a > b, false otherwise
bool gt_uint512(uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   LTE                         │
  └───────────────────────────────┘
 */

// less than or equal comparison for uint128_ts
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a <= b, false otherwise
bool lte_uint128(uint128_t *a, uint128_t *b);

// less than or equal comparison for uint256_ts
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return true if a <= b, false otherwise
bool lte_uint256(uint256_t *a, uint256_t *b);

// less than or equal comparison for uint512_ts
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return true if a <= b, false otherwise
bool lte_uint512(uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   GTE                         │
  └───────────────────────────────┘
 */

// greater than or equal comparison for uint128_ts
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a >= b, false otherwise
bool gte_uint128(uint128_t *a, uint128_t *b);

// greater than or equal comparison for uint256_ts
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return true if a >= b, false otherwise
bool gte_uint256(uint256_t *a, uint256_t *b);

// greater than or equal comparison for uint512_ts
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return true if a >= b, false otherwise
bool gte_uint512(uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   BITS                        │
  └───────────────────────────────┘
 */

//
// @param number:
// @return
uint32_t bits_uint128(uint128_t *number);

//
// @param number:
// @return
uint32_t bits_uint256(uint256_t *number);

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   Arithmetic                                                               │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   ADD                         │
  └───────────────────────────────┘
 */

// add two uint128_t
// @param dest: where to store the sum of a and b
// @param a: the first uint128_t to add
// @param b: the second uint128_t to add
void add_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// add two uint256_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint256_t to add
// @param b: the second uint256_t to add
void add_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// add two uint512_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint512_t to add
// @param b: the second uint512_t to add
void add_uint512(uint512_t *dest, uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   SUB                         │
  └───────────────────────────────┘
 */

// subtract two uint128_ts
// @param dest: where to store the difference of a and b
// @param a: the first uint128_t to subtract
// @param b: the second uint128_t to subtract
void sub_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// subtract two uint256_ts
// @param dest: where to store the difference of a and b
// @param a: the first uint256_t to subtract
// @param b: the second uint256_t to subtract
void sub_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// subtract two uint512_ts
// @param dest: where to store the difference of a and b
// @param a: the first uint512_t to subtract
// @param b: the second uint512_t to subtract
void sub_uint512(uint512_t *dest, uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   MUL                         │
  └───────────────────────────────┘
 */

// multiply two uint128_ts
// @param dest: where to store the product of a and b
// @param a: the first uint128_t to multiply
// @param b: the second uint128_t to multiply
void mul_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// multiply two uint256_ts
// @param dest: where to store the product of a and b
// @param a: the first uint256_t to multiply
// @param b: the second uint256_t to multiply
void mul_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// multiply two uint512_ts
// @param dest: where to store the product of a and b
// @param a: the first uint512_t to multiply
// @param b: the second uint512_t to multiply
void mul_uint512(uint512_t *dest, uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   DIV                         │
  └───────────────────────────────┘
 */

// divide + mod two uint128_ts
// @param destDiv: where to store the quotient of a and b
// @param destMod: where to store the remainder of a and b
// @param a: the uint128_t to divide
// @param b: the uint128_t to divide with
void divmod_uint128(uint128_t *destDiv, uint128_t *destMod, uint128_t *a,
                    uint128_t *b);

// divide + mod two uint256_ts
// @param destDiv: where to store the quotient of a and b
// @param destMod: where to store the remainder of a and b
// @param a: the  uint256_t to divide
// @param b: the uint256_t to divide with
void divmod_uint256(uint256_t *destDiv, uint256_t *destMod, uint256_t *a,
                    uint256_t *b);

// divide + mod two uint512_ts
// @param destDiv: where to store the quotient of a and b
// @param destMod: where to store the remainder of a and b
// @param a: the  uint512_t to divide with
// @param b: the  uint512_t to divide with
void divmod_uint512(uint512_t *destDiv, uint512_t *destMod, uint512_t *a,
                    uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   EXP                         │
  └───────────────────────────────┘
 */

// exponentiate two uint128_ts
// @param dest: where to store the result of a^b
// @param a: the first uint128_t to exponentiate
// @param b: the second uint128_t to exponentiate
void exp_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// exponentiate two uint256_ts
// @param dest: where to store the result of a^b
// @param a: the first uint256_t to exponentiate
// @param b: the second uint256_t to exponentiate
void exp_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// exponentiate two uint512_ts
// @param dest: where to store the result of a^b
// @param a: the first uint512_t to exponentiate
// @param b: the second uint512_t to exponentiate
void exp_uint512(uint512_t *dest, uint512_t *a, uint512_t *b);

/*
  ┌───────────────────────────────┐
  │   EQUAL                       │
  └───────────────────────────────┘
 */

// check if two uint128_ts are equal
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a == b, false otherwise
bool equal_uint128(uint128_t *a, uint128_t *b);

// check if two uint256_ts are equal
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return true if a == b, false otherwise
bool equal_uint256(uint256_t *a, uint256_t *b);

// check if two uint512_ts are equal
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return true if a == b, false otherwise
bool equal_uint512(uint512_t *a, uint512_t *b);

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   MISC                                                                     │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   PRINT DECIMAL               │
  └───────────────────────────────┘
 */

// print a uint128_t in decimal
// @param a: the uint128_t to print
// @param newline: optional newline
void print_uint128(uint128_t *a, bool newline);

// print a uint256_t in decimal
// @param a: the uint256_t to print
// @param newline: optional newline
void print_uint256(uint256_t *a, bool newline);

// print a uint512_t in decimal
// @param a: the uint512_t to print
// @param newline: optional newline
void print_uint512(uint512_t *a, bool newline);

/*
  ┌───────────────────────────────┐
  │   PRINT HEX                   │
  └───────────────────────────────┘
 */

// printf a uint128_t in hexadecimal
// @param src: the uint128_t to print
// @param newline: optional newline
void print_hex_uint128(uint128_t *src, bool newline);

// printf a uint256_t in hexadecimal
// @param src: the uint256_t to print
// @param newline: optional newline
void print_hex_uint256(uint256_t *src, bool newline);

// printf a uint512_t in decimal
// @param src: the uint512_t to print
// @param newline: optional newline
void print_hex_uint512(uint512_t *a, bool newline);

/*
  ┌───────────────────────────────┐
  │  GET ELEMENT FROM GIVEN INDEX │
  └───────────────────────────────┘
 */

// get uint64_t at a given index in uint128_t
// @param src: the uint128_t to get the index from
// @param index: the index to get
// @return the uint64_t at the given index
uint64_t get_element_uint128(uint128_t *src, int index);

// get uint64_t at a given index in uint256_t
// @param src: the uint256_t to get the index from
// @param index: the index to get
// @return the uint64_t at the given index
uint64_t get_element_uint256(uint256_t *src, int index);

#endif