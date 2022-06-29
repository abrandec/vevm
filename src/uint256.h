#ifndef _UINT256_UINT_H_
#define _UINT256_UINT_H_

#include <inttypes.h>
#include <stdbool.h>

// Type definitions
// uint128_t by packing 2 uint64_ts into a single struct
typedef struct uint128_t {
  uint64_t e[2]; // element
} uint128_t;

// uint256_t by packing 2 uint128_ts into a single struct
typedef struct uint256_t {
  uint128_t e[2]; // element
} uint256_t;

//                                                      //
//                        MACROS                        //
//                                                      //

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

//                                                         //
//                        FUNCTIONS                        //
//                                                         //

// create a uint128_t initialized with a uint64_t
// @param: the uint64_t to initialize each uint64_t element of the uint128_t to
// @return the newly created uint128_t
uint128_t init_uint128(uint64_t a);
// create a uint256_t initialized with uint64_t
// @param: the uint64_t to initialize each uint64_t element of the uint256_t to
// @return the initted uint256_t
uint256_t init_uint256(uint64_t a);

// set elements in a uint128_t
// @param a: the 1st element to change (uint128_t.uint64_t[0])
// @param b: the 2nd element to change (uint128_t.uint64_t[1])
// @return the newly created uint128_t
uint128_t set_uint128(uint64_t a, uint64_t b);

// set elements in a uint256_t
// @param a: the 1st element to set (uint256_t.uint128_t[0].uint64_t[0])
// @param b: the 2nd element to set (uint256_t.uint128_t[0].uint64_t[1])
// @param c: the 3rd element to set (uint256_t.uint128_t[1].uint64_t[0])
// @param d: the 4th element to set (uint256_t.uint128_t[1].uint64_t[1])
// @return a newly created uint256_t
uint256_t set_uint256(uint64_t a, uint64_t b, uint64_t c, uint64_t d);

// copy uint128_t into another uint128_t
// @param dest: the destination uint128_t to copy src into
// @param src: the source uint128_t to copy into dest
void copy_uint128(uint128_t *dest, uint128_t *src);

// copy uint256_t into another uint256_t
// @param dest: the destination uint256_t to copy src into
// @param src: the source uint256_t to copy into dest
void copy_uint256(uint256_t *dest, uint256_t *src);

// set a uint128_t to 0
// @param dest: the uint128_t to set to 0
void clear_uint128(uint128_t *dest);

// set a uint256_t to 0
// @param dest: the uint256_t to set to 0
void clear_uint256(uint256_t *dest);

///                                                       ///
///                       Bit stuff                       ///

// shift a uint128_t left by n bits
// @param dest: where to store the shifted uint128_t
// @param src: the uint128_t to shift
// @param shift: the number of bits to shift
void shiftl_uint128(uint128_t *dest, uint128_t *src, int shift);

// shift a uint256_t left by n bits
// @param dest: where to store the shifted uint256_t
// @param src: the uint256_t to shift
// @param shift: the number of bits to shift
void shiftl_uint256(uint256_t *dest, uint256_t *src, int shift);

// shift a uint128_t right by n bits
// @param dest: where to store the shifted uint128_t
// @param src: the uint128_t to shift
// @param shift: the number of bits to shift
void rshift_uint128(uint128_t *dest, uint128_t *src, int shift);

// shift a uint256_t right by n bits
// @param dest: where to store the shifted uint128_t
// @param src: the uint256_t to shift
// @param shift: the number of bits to shift
void rshift_uint256(uint256_t *dest, uint256_t *src, int shift);

// and two uint128_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint128_t to and
// @param b: the second uint128_t to and
void and_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// and two uint256_ts
// @param dest: where to store the sum of a and b
// @param a: the first uint256_t to and
// @param b: the second uint256_t to and
void and_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

void or_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

void or_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

bool lt_uint128(uint128_t *a, uint128_t *b);

bool lt_uint256(uint256_t *a, uint256_t *b);

bool gt_uint128(uint128_t *a, uint128_t *b);

bool gt_uint256(uint256_t *a, uint256_t *b);

///                                                        ///
///                       Arithmetic                       ///

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

// check if two uint128_t are equal
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a == b, false otherwise
bool equal_uint128(uint128_t *a, uint128_t *b);

// check if two uint256_t are equal
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return true if a == b, false otherwise
bool equal_uint256(uint256_t *a, uint256_t *b);

///                                                      ///
///                       Printing                       ///

// printf a uint128_t in hexadecimal
// @param a: the uint128_t to print
void print_hex_uint128(uint128_t *a);

// printf a uint256_t in hexadecimal
// @param a: the uint256_t to print
void print_hex_uint256(uint256_t *a);

///                                                          ///
///                           Misc                           ///

// uint128_tostring

#endif