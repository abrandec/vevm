#include "cmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void box_cmd(void *data, int width, int height, int data_type) {
    switch (data_type) {
        // char
        case 0:
            // calculate width
            printf("box: %d %d %d\n", width, height, data_type);
            break;
        // int
        case 1:
            // calculate width
            printf("box: %d %d %d\n", width, height, data_type);
            break;
        case 2:
            // calculate width
            printf("box: %d %d %d\n", width, height, data_type);
            break;
        // uint256_t
        case 3:
            // calculate width
            printf("box: %d %d %d\n", width, height, data_type);
            break;
    }
}

void row_box_cmd(void *data, int rows, int width, int height, int data_type) {}

void col_box_cmd(void *data, int columns, int width, int height, int data_type) {}

void grid_box_cmd(void *data, int columns, int width, int height, int data_type) {}

//void print_stack(List *stack) {}

void print_stack_peak(List *stack, int index) {}

void print_test_result(char test_name, bool result) {
  if (result) {
    box_cmd(test_name, 10, 10, 0);
        printf("PASS\n");
  } else {
    printf("FAIL\n");
  }
}
