#ifndef HEX_UTILS_H
#define HEX_UTILS_H

#include <inttypes.h>

/*
  ┌───────────────────────────────┐
  │   HEX CHAR TO UINT            │
  └───────────────────────────────┘
 */

// convert a hex string to a uint64_t
// @param hex: the hex string to convert (max length of 16 characters)
// @param offset: the offset to start at in the hex string
// @param length: the length of the hex string to convert
// @return the converted uint64_t
uint64_t hex_char2uint(char *hex, int offset, int length);

/*
  ┌───────────────────────────────┐
  │   UINT2HEX                    │
  └───────────────────────────────┘
 */

// convert an uint64_t to a hex string
// @param val: the uint64_t to convert
// @return the converted hex string
char *uint2hex(uint64_t *val);

/*
  ┌───────────────────────────────┐
  │   UINT2HEX                    │
  └───────────────────────────────┘
 */

// reverse a string up to a given length
// @param str: the string to reverse
// @param length: the length of the string
void reverse_string(char *str, uint32_t length);

/*
  ┌───────────────────────────────┐
  │   HEX LENGTH                  │
  └───────────────────────────────┘
 */

// get the length of a uint64_t in base 16 rounded up
// @param src: the uint64_t to get the length of
// @return the length of the hex string in bytes rounded up
int hex_uint64_len(uint64_t *src);

#endif