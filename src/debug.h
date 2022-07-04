#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "bigint.h"
#include "stack.h"

// custom error handling
// @param err_msg: error message
void custom_error(char err_msg[]);

// print a given buffer
// @param buffer: the buffer to print
// @param buff_name[]: name of buffer to print
// @param length: index to stop printing at
void print_buffer(uint256_t buffer[], char buff_name[], int length);

/* Stack debugging */

// printfs the entire stack
// @param stack: the stack to print
void stack_print(List *stack);

// printf a specific stack index
// @param stack: which to stack to point to
// @param index: index to print from stack
void stack_peak_print(List *stack, int index);

#endif