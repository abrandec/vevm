#ifndef ASSERT_H
#define ASSERT_H

#include "../../src/common/cmd/cmd.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

// print a message with pass/fail status
// @param msg[]: the message to print
// @param condition: the condition of the test
// @return: the passed condition
bool assert_msg(char msg[], bool condition);

// assert that two uint64_ts are equal
// @param a: the first uint64_t to compare
// @param b: the second uint64_t to compare
// @return: true if a == b, false otherwise
bool assert_eq(uint64_t a, uint64_t b);

// assert that two uint64_ts are equal
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return: true if a == b, false otherwise
bool assert_eq_uint128(uint128_t a, uint128_t b);

// assert that two uint256_ts are equal
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return: true if a == b, false otherwise
bool assert_eq_uint256(uint256_t a, uint256_t b);

// assert that two uint512_ts are equal
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return: true if a == b, false otherwise
bool assert_eq_uint512(uint512_t a, uint512_t b);

// assert that two uint64_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint64_t to compare
// @param b: the second uint64_t to compare
// @return: true if a == b, false otherwise
bool assert_eq_msg(char msg[], uint64_t a, uint64_t b);

// assert that two uint128_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
// @return: true if a == b, false otherwise
bool assert_eq_128_msg(char msg[], uint128_t a, uint128_t b);

// assert that two uint256_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
// @return: true if a == b, false otherwise
bool assert_eq_256_msg(char msg[], uint256_t a, uint256_t b);

// assert that two uint512_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
// @return: true if a == b, false otherwise
bool assert_eq_512_msg(char msg[], uint512_t a, uint512_t b);

// uint64_t pseudo number generator
// @param min: the minimum value to generate
// @param max: the maximum value to generate
// @return a pseudo random number between a given min and max
uint64_t rand_num(uint64_t min, uint64_t max);

// uint128_t psudo number generator
// @param min: the minimum value to generate
// @param max: the maximum value to generate
// @return a pseudo random number between a given min and max
uint128_t rand_num_uint128(uint128_t min, uint128_t max);

// uint256_t psudo number generator
// @param min: the minimum value to generate
// @param max: the maximum value to generate
// @return a pseudo random number between a given min and max
uint256_t rand_num_uint256(uint256_t min, uint256_t max);

// uint512_t psudo number generator
// @param min: the minimum value to generate
// @param max: the maximum value to generate
// @return a pseudo random number between a given min and max
uint512_t rand_num_uint512(uint512_t min, uint512_t max);

// assert that all the values in an array of true
// @param msg[]: the message to print
// @param arr[]: the array to check
// @param len: the length of the array
// @return true if all the values in the array are true, false otherwise
bool assert_bool_array_msg(char msg[], bool arr[], size_t len);

#endif