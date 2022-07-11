#ifndef CMD_H
#define CMD_H

#include "../../core/stack/stack.h"
#include "color_codes.h"

// print a specific element of the stack from a given index
// @param index: index to print from stack
void print_stack_peak(int index);

// print a prettified test result
// @param test_name: name of test
// @param result: result of test
void print_test_result(char test_name, bool result);

#endif