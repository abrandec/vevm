#ifndef _debug_H_
#define _debug_H_

// for custom error handling in vm (e.g. exit(1), run another program, etc.)
// @param err_msg: error message for easier debugging
void custom_error(char err_msg[]);

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