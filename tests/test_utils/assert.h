#ifndef ASSERT_H
#define ASSERT_H

#include "../../src/common/cmd/cmd.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ASSERT_INT8_T 0x01
#define ASSERT_INT16_T 0x02
#define ASSERT_INT32_T 0x03
#define ASSERT_INT64_T 0x04

#define ASSERT_UINT8_T 0x05
#define ASSERT_UINT16_T 0x06
#define ASSERT_UINT32_T 0x07
#define ASSERT_UINT64_T 0x08

#define ASSERT_UINT128_T 0x09
#define ASSERT_UINT256_T 0x0A
#define ASSERT_UINT512_T 0x0B

#define ASSERT_FLOAT_T 0x0C
#define ASSERT_DOUBLE_T 0x0D

#define ASSERT_STRING_T 0x0E
#define ASSERT_CHAR_T 0x0F

#define ASSERT_BOOL_T 0x10

typedef struct error_t {
  // __func__
  const char *main_function;
  // message to print for assertation
  char *message;
  // data type of expected and actual
  uint8_t data_type;
  // true for base10, false for base16
  bool base10;
  // true for signed integer, false for unsigned integer
  bool _signed;
  // any data type for asserting
  void *expected;
  void *actual;
} error_t;

// print a message with pass/fail status
// @param msg[]: the message to print
// @param condition: the condition of the test
// @return the passed or failed condition
bool assert_msg(const char msg[], bool condition);

// print a function with a passing assertation
// @param function[]: the function to print
void assert_pass(const char function[]);

// set an error without making a mess in your function
// @param error: the error to set
// @param main_func: the main function of the test
// @param msg[]: the message to print
// @param data_type: the data type of the test
// @param base10: true for base10, false for base16
// @param _signed: true for signed integer, false for unsigned integer
// @return true for initializing the error
bool set_error(error_t *error, const char main_func[], char msg[], uint8_t data_type, bool base10, bool _signed);

// return an error from an assertation
// @param error: the error to return
// @return false
bool assert_error(error_t *error);

// assert that two uint64_ts are equal
// @param a: the first uint64_t to compare
// @param b: the second uint64_t to compare
// @return true if a == b, false otherwise
bool assert_eq(uint64_t a, uint64_t b);

// assert that two uint64_ts are equal
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a == b, false otherwise
bool assert_eq_uint128(uint128_t a, uint128_t b);

// assert that two uint256_ts are equal
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return true if a == b, false otherwise
bool assert_eq_uint256(uint256_t a, uint256_t b);

// assert that two uint512_ts are equal
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return true if a == b, false otherwise
bool assert_eq_uint512(uint512_t a, uint512_t b);

// assert that two uint64_ts are equal with a message
// @param func[]: the function name
// @param msg[]: the message to print
// @param expected: the expected value to compare
// @param actual: the actual value to compare
// @return true if expected == actual, false otherwise
bool assert_eq_msg(const char func[], const char msg[], uint64_t expected,
                   uint64_t actual);

// assert that two uint128_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return true if a == b, false otherwise
bool assert_eq_128_msg(char msg[], uint128_t a, uint128_t b);

// assert that two uint256_ts are equal with a message
// @param func[]: the function name
// @param msg[]: the message to print
// @param expected: the expected value to compare
// @param actual: the actual value to compare
// @return true if expected == actual, false otherwise
bool assert_eq_256_msg(const char func[], const char msg[], uint256_t expected,
                       uint256_t actual);

// assert that two uint512_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return true if a == b, false otherwise
bool assert_eq_512_msg(char msg[], uint512_t a, uint512_t b);

// assert that two strings are equal
// @param func[]: the function name
// @param msg[]: the message to print
// @param expected: the expected string to compare
// @param actual: the actual string to compare
// @return true if expected == actual, false otherwise
bool assert_char(const char func[], const char msg[], char *expected,
                 char *actual);

// uint64_t pseudo number generator
// @return a pseudo random number
uint64_t rand_num(void);

// assert that all the values in an array of true
// @param msg[]: the message to print
// @param arr[]: the array to check
// @param len: the length of the array
// @return true if all the values in the array are true, false otherwise
bool assert_bool_array_msg(char msg[], bool arr[], size_t len);

#endif