#ifndef BIG_INT_H
#define BIG_INT_H

#include <inttypes.h>
#include <stdbool.h>

////////////////////////
/// TYPE DEFINITIONS ///
////////////////////////

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

//                                                      //
//                        MACROS                        //
//                                                      //

/////////////////////////////////////////
/// MAXMIMUM VALUES OF EACH UINT TYPE ///
/////////////////////////////////////////

#define UINT128_MAX ((uint128_t){.e = {UINT64_MAX, UINT64_MAX}})
#define UINT256_MAX ((uint256_t){.e = {UINT128_MAX, UINT128_MAX}})
#define UINT512_MAX ((uint512_t){.e = {UINT256_MAX, UINT256_MAX}})

////////////////////////////////
/// ELEMENT ACCESSING MACROS ///
////////////////////////////////

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

////////////
/// INIT ///
////////////

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

////////////////
/// INIT ALL ///
////////////////

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

/////////////////////
/// CHANGE VALUES ///
/////////////////////

// change elements in an already initialized uint128_t
// @param dest: the uint128_t to change
// @param a: the 1st element to change (uint128_t.uint64_t[0])
// @param b: the 2nd element to change (uint128_t.uint64_t[1])
void change_uint128(uint128_t *dest, uint64_t a, uint64_t b);

// change elements in an already initialized uint256_t
// @param dest: the uint256_t to change
// @param a: the 1st element to change (uint256_t.uint128_t[0].uint64_t[0])
// @param b: the 2nd element to change (uint256_t.uint128_t[0].uint64_t[1])
// @param c: the 3rd element to change (uint256_t.uint128_t[1].uint64_t[0])
// @param d: the 4th element to change (uint256_t.uint128_t[1].uint64_t[1])
void change_uint256(uint256_t *dest, uint64_t a, uint64_t b, uint64_t c,
                    uint64_t d);

// check change_uint256 && change_uint128 for reference
void change_uint512(uint512_t *dest, uint64_t a, uint64_t b, uint64_t c,
                    uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h);

////////////
/// COPY ///
////////////

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

//////////////////
/// CLEAR UINT ///
//////////////////

// set a uint128_t to 0
// @param dest: the uint128_t to set to 0
void clear_uint128(uint128_t *dest);

// set a uint256_t to 0
// @param dest: the uint256_t to set to 0
void clear_uint256(uint256_t *dest);

// set a uint512_t to 0
// @param dest: the uint512_t to set to 0
void clear_uint512(uint512_t *dest);

///                                                       ///
///                       Bit stuff                       ///

///////////
/// SHL ///
///////////

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

///////////
/// SHR ///
///////////

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

///////////
/// AND ///
///////////

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

//////////
/// OR ///
//////////

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

///////////
/// XOR ///
///////////

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

//////////
/// LT ///
//////////

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

//////////
/// GT ///
//////////

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

///////////
/// LTE ///
///////////

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

///////////
/// GTE ///
///////////

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

///                                                        ///
///                       Arithmetic                       ///

///////////
/// ADD ///
///////////

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

///////////
/// SUB ///
///////////

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

/////////////
/// EQUAL ///
/////////////

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

///                                                          ///
///                           Misc                           ///

////////////////
/// PRINTING ///
////////////////

// printf a uint128_t in hexadecimal
// @param src: the uint128_t to print
void print_hex_uint128(uint128_t *a);

// printf a uint256_t in hexadecimal
// @param src: the uint256_t to print
void print_hex_uint256(uint256_t *a);

// printf a uint512_t in decimal
// @param src: the uint512_t to print
void print_hex_uint512(uint512_t *a);

//////////////////
/// HEX LENGTH ///
//////////////////

// get byte length of a uint in base 16
// @param src: uint64_t to get length of
// @return byte length of x rounded up
int hex_length(uint64_t *src);

// get byte length of a uint128 in base 16
// @param src: uint128 to get length of
// @return byte length of src rounded up
int hex_length_uint128(uint128_t *src);

// get byte length of a uint256 in base 16
// @param src: uint256 to get length of
// @return byte length of src rounded up
int hex_length_uint256(uint256_t *src);

// get byte length of a uint512 in base 16
// @param src: uint512 to get length of
// @return byte length of src rounded up
int hex_length_uint512(uint512_t *src);

////////////////
/// GET UINT ///
////////////////

// get a uint64_t from a uint256_t
// @param src: the uint256_t to get the uint64_t from
// @param index: the index of the uint64_t to get
// @return the uint64_t at index
uint64_t get_uint64(uint256_t *src, int *index);

#endif