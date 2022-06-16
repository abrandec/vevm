#ifndef _EXTENDED_UINT_H_
#define _EXTENDED_UINT_H_

#include <inttypes.h>
#include <stdbool.h>

// Type definitions
// e == element
// uint128_t by packing 2 uint64_t into a single struct
typedef struct uint128_t {
  uint64_t e[2];
} uint128_t;

// uint256_t by packing 4 uint64_t into a single struct
typedef struct uint256_t {
  uint128_t e[2];
} uint256_t;

// macros for accessing elements
// E for e, # for index
// e.g. E_0(a) is the first e of uint256_t a
#define E_0(x) x->e[0]
#define E_1(x) x->e[1]

#define E0(x) x.e[0]
#define E1(x) x.e[1]

// create a uint128_t initialized to 0
// @return a pointer to the initialized uint128_t
uint128_t *create_uint128(void);

// create a uint256_t initialized to 0
// @return a pointer to the initialized uint256_t
uint256_t *create_uint256(void);

// create a uint128_t using two 64-bit numbers
// @param a: the first 64-bit number
// @param b: the second 64-bit number
// @return a pointer to the newly created uint128_t
uint128_t *set_uint128(uint64_t a, uint64_t b);

// create a uint256_t using four 64-bit numbers
// @param a: the first 64-bit number
// @param b: the second 64-bit number
// @param c: the third 64-bit number
// @param d: the fourth 64-bit number
// @return a pointer to the newly created uint256_t
uint256_t *set_uint256(uint64_t a, uint64_t b, uint64_t c, uint64_t d);

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

// check if two uint128_t are equal
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return: true if a == b, false otherwise
bool equal_uint128(uint128_t *a, uint128_t *b);

// check if two uint256_t are equal
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return: true if a == b, false otherwise
bool equal_uint256(uint256_t *a, uint256_t *b);

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

// add two uint128_t
// @param dest: where to store the sum of a and b
// @param a: the first uint128_t to add
// @param b: the second uint128_t to add
void add_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// add two uint256_t
// @param dest: where to store the sum of a and b
// @param a: the first uint256_t to add
// @param b: the second uint256_t to add
void add_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// subtract two uint128_t
// @param dest: where to store the difference of a and b
// @param a: the first uint128_t to subtract
// @param b: the second uint128_t to subtract
void sub_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// subtract two uint256_t
// @param dest: where to store the difference of a and b
// @param a: the first uint256_t to subtract
// @param b: the second uint256_t to subtract
void sub_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// and two uint128_t
// @param dest: where to store the sum of a and b
// @param a: the first uint128_t to and
// @param b: the second uint128_t to and
void and_uint128(uint128_t *dest, uint128_t *a, uint128_t *b);

// and two uint256_t
// @param dest: where to store the sum of a and b
// @param a: the first uint256_t to and
// @param b: the second uint256_t to and
void and_uint256(uint256_t *dest, uint256_t *a, uint256_t *b);

// printf a uint128_t in hexadecimal
// @param a: the uint128_t to print
void print_hex_uint128(uint128_t *a);

// printf a uint256_t in hexadecimal
// @param a: the uint256_t to print
void print_hex_uint256(uint256_t *a);

#endif