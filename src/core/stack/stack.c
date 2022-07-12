#include "stack.h"

#include "../../common/math/bigint/bigint.h"
#include "../../errors/errors.h"
#include "../config.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include "../../debug/debug.h"
#endif

// for keeping track of stack length
static int stack_len = 0;

static uint256_t stack[MAX_STACK_DEPTH];

/*
  ┌───────────────────────────────┐
  │   STACK PUSH                  │
  └───────────────────────────────┘
 */

void stack_push(uint256_t *val) { 
if (stack_len > MAX_STACK_DEPTH - 1) {
 custom_error(STACK_OVERFLOW);
} else {
  copy_uint256(&stack[stack_len], val);
 ++stack_len;
}
}
/*
  ┌───────────────────────────────┐
  │   STACK POP                   │
  └───────────────────────────────┘
 */

void stack_pop(void) {
  if (stack_len == 0) {
    custom_error(STACK_UNDERFLOW);
  } else {
    --stack_len;
    zero_uint256(&stack[stack_len]);
  }
}

/*
  ┌───────────────────────────────┐
  │   STACK SWAP                  │
  └───────────────────────────────┘
 */

void stack_swap(int index) {
  if (stack_len == 0 || index > stack_len - 1 || index < 0) {
    custom_error(STACK_UNDERFLOW);
  } else {
    uint256_t temp = stack[stack_len - 1];
    stack[stack_len - 1] = stack[index];
    stack[index] = temp;
  }
}

/*
  ┌───────────────────────────────┐
  │   STACK PEAK                  │
  └───────────────────────────────┘
 */

uint256_t stack_peak(int index) {
  uint256_t val = init_uint256(0);

  int stack_l = stack_length();
  index > stack_len - 1 || index < 0 ? custom_error(STACK_UNDERFLOW) : 0;
  return stack[index];
}

/*
  ┌───────────────────────────────┐
  │   STACK LENGTH                │
  └───────────────────────────────┘
 */

int stack_length(void) { return stack_len; }

/*
  ┌───────────────────────────────┐
  │   STACK RESET                 │
  └───────────────────────────────┘
 */

void stack_reset(void) { stack_len = 0; }