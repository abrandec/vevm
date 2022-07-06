#ifndef CMD_H
#define CMD_H

#include "../../core/stack/stack.h"
#include "color_codes.h"

// box for printing
// @param data: the data to print
// @param length: the length of the string
// @param height: the height of the boxes
// @param data_type: the data type of the data (refer to cmd.h for codes)
void box_cmd(void *data, int width, int height, int data_type);

// rows of box for printing
// @param data: the string to print
// @param length: the length of the string
// @param height: the height of the boxes
// @param data_type: the data type of the data (refer to cmd.h for codes)
void row_box_cmd(void *data, int rows, int width, int height, int data_type);

// columns of boxes for printing
// @param data: the data to print
// @param length: the length of the boxes
// @param height: the height of the boxes
// @param data_type: the data type of the data (refer to cmd.h for codes)
void col_box_cmd(void *data, int columns, int width, int height, int data_type);

// grid box for printing
// @param data: the data to print
// @param length: the length of the boxes
// @param height: the height of the boxes
// @param data_type: the data type of the data (refer to cmd.h for codes)
// @param data_type: the data type of the data (refer to cmd.h for codes)
void grid_box_cmd(void *data, int columns, int width, int height,
                  int data_type);

// print the entire stack
// @param stack: the stack to print
// void print_stack(List *stack);

// print a specific element of the stack from a given index
// @param stack: which to stack to point to
// @param index: index to print from stack
void print_stack_peak(List *stack, int index);

// print a prettified test result
// @param test_name: name of test
// @param result: result of test
void print_test_result(char test_name, bool result);

#endif