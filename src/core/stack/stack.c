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

static uint256_t stackk[MAX_STACK_DEPTH];

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   NODE FUNCTIONS                                                           │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   NODE CREATE                 │
  └───────────────────────────────┘
 */

Node *Node_create(void) {

  Node *node = malloc(sizeof(Node));

  assert(node != NULL);

  node->data = 0;

  node->next = NULL;

  return node;
}

/*
  ┌───────────────────────────────┐
  │   NODE DESTROY                │
  └───────────────────────────────┘
 */

void Node_destroy(Node *node) {
  if (node == NULL) {
    custom_error(stack_null_err);
  } else {
    free(node);
  }
}

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   STACK FUNCTIONS                                                          │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

/*
  ┌───────────────────────────────┐
  │   STACK CREATE                │
  └───────────────────────────────┘
 */

List *stack_create(void) {
  List *stack = malloc(sizeof(List));
  Node *node = Node_create();

  stack->first = node;

  return stack;
}

/*
  ┌───────────────────────────────┐
  │   STACK DESTROY               │
  └───────────────────────────────┘
 */

void stack_destroy(List *stack) {
  if (stack == NULL) {
    custom_error(stack_null_err);
  } else {
    Node *node = stack->first;
    Node *next;

    stack_len = 0;

    while (node != NULL) {
      next = node->next;
      free(node);
      node = next;
    }

    free(stack);
  }
}

/*
  ┌───────────────────────────────┐
  │   STACK PUSH                  │
  └───────────────────────────────┘
 */

void stack_push(List *stack, uint256_t *val) {
  if (stack_length(stack) > MAX_STACK_DEPTH - 1) {
    custom_error(stack_overflow_err);
  } else if (stack == NULL) {
    custom_error(stack_null_err);
  } else {
    ++stack_len;
    Node *node = stack->first;
    while (node->next != NULL) {
      node = node->next;
    }

    uint256_t *value = malloc(sizeof(uint256_t));

    copy_uint256(value, val);

    node->data = value;

    node->next = Node_create();
  }
}

/*
  ┌───────────────────────────────┐
  │   STACK POP                   │
  └───────────────────────────────┘
 */

void stack_pop(List *stack) {

  if (stack_length(stack) == 0) {
    custom_error(stack_underflow_err);
  } else {
    int index = stack_length(stack) - 1;
    --stack_len;
    if (index == 0) {
      Node *node = stack->first;
      stack->first = stack->first->next;
      uint256_t *data = node->data;
      free(data);
      Node_destroy(node);
    } else {
      Node *before = stack->first;
      while (index > 0) {
        before = before->next;
        --index;
      }
      Node *node = before->next;
      before->next = before->next->next;
      uint256_t *data = node->data;
      free(data);
      Node_destroy(node);
    }
  }
}

/*
  ┌───────────────────────────────┐
  │   STACK SWAP                  │
  └───────────────────────────────┘
 */

void stack_swap(List *stack, int index) {
  int stack_l = stack_length(stack) - 1;

  if (stack == NULL || index == 0 || index > 15 || index > stack_len) {
    custom_error(inaccessable_element_err);
  } else {
    uint256_t data2swap;

    Node *before = stack->first;
    Node *after = stack->first;

    while (stack_l != index) {
      before = before->next;
      --stack_l;
    }

    // top of stack element
    copy_uint256(&data2swap, after->data);

    // element to swap with
    copy_uint256(after->data, before->data);

    copy_uint256(before->data, &data2swap);
  }
}

/*
  ┌───────────────────────────────┐
  │   STACK PEAK                  │
  └───────────────────────────────┘
 */

uint256_t stack_peak(List *stack, int index) {
  uint256_t val = init_uint256(0);

  int stack_l = stack_length(stack);

  if (stack == NULL || stack_l == 0 || index > stack_l - 1 || index < -1) {
    // slightly unhelpful error message!
    custom_error(inaccessable_element_err);
    return val;
  } else {
    Node *node = stack->first;
    int i;

    for (i = 0; i < index; ++i) {
      node = node->next;
    }

    copy_uint256(&val, node->data);
    return val;
  }
}

/*
  ┌───────────────────────────────┐
  │   STACK LENGTH                │
  └───────────────────────────────┘
 */

int stack_length(List *stack) {
  if (stack == NULL) {
    custom_error(stack_null_err);
    return 0;
  } else {
    Node *node = stack->first;
    return stack_len;
  }
}