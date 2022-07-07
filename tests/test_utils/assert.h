#ifndef ASSERT_H
#define ASSERT_H

#include "../../src/common/cmd/cmd.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// print a message with pass/fail status
// @param msg[]: the message to print
// @param status: the status of the test
bool assert_msg(char msg[], bool condition)
{
    if (condition)
    {
        printf(GREEN "%s - PASS\n" RESET, msg);

        return true;
    }
    else
    {
        printf(RED "%s - FAIL\n" RESET, msg);
        return false;
    }
}

// assert that two uint64_ts are equal
// @param a: the first uint64_t to compare
// @param b: the second uint64_t to compare
bool assert_eq(uint64_t a, uint64_t b) {
    return (a == b);
}

// assert that two uint64_ts are equal
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
bool assert_eq_uint128(uint128_t a, uint128_t b) {
    return (equal_uint128(&a, &b));
}

// assert that two uint256_ts are equal
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
bool assert_eq_uint256(uint256_t a, uint256_t b) {
    return (equal_uint256(&a, &b));
}

// assert that two uint512_ts are equal
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
bool assert_eq_uint512(uint512_t a, uint512_t b) {
    return (equal_uint512(&a, &b));
}

// assert that two uint64_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint64_t to compare
// @param b: the second uint64_t to compare
bool assert_eq_msg(char msg[], uint64_t a, uint64_t b)
{
    return assert_msg(msg, a == b);
}

// assert that two uint128_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint128_t to compare
// @param b: the second uint128_t to compare
bool assert_eq_128_msg(char msg[], uint128_t a, uint128_t b)
{
    return assert_msg(msg, equal_uint128(&a, &b));
}

// assert that two uint256_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint256_t to compare
// @param b: the second uint256_t to compare
bool assert_eq_256_msg(char msg[], uint256_t a, uint256_t b)
{
    return assert_msg(msg, equal_uint256(&a, &b));
}

// assert that two uint512_ts are equal with a message
// @param msg[]: the message to print
// @param a: the first uint512_t to compare
// @param b: the second uint512_t to compare
bool assert_eq_512_msg(char msg[], uint512_t a, uint512_t b)
{
    return assert_msg(msg, equal_uint512(&a, &b));
}

#endif