#include "stack.h"
#include "bigint.h"
#include "config.h"
#include "debug.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for keeping track of stack length
static int stack_len = 0;

// create a node
// @return a ptr to the newly created node
Node *Node_create(void) {

  Node *node = malloc(sizeof(Node));

  assert(node != NULL);

  node->data = 0;

  node->next = NULL;

  return node;
}

// destroy a node
// @param node: the node to destroy
void Node_destroy(Node *node) {
  if (node == NULL) {
    char err_msg[50] = "Node_destroy: node is NULL\n";
    custom_error(err_msg);
  } else {
    free(node);
  }
}

// create a new stack
// @returns a new stack
List *stack_create(void) {
  List *stack = malloc(sizeof(List));
  Node *node = Node_create();

  stack->first = node;

  return stack;
}

// destroy the stack
// @param stack: the stack to destroy
void stack_destroy(List *stack) {
  if (stack == NULL) {
    char err_msg[50] = "stack_destroy: stack is NULL\n";
    custom_error(err_msg);
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

// push a new e onto the top of the stack
// @param stack: the stack to push onto
// @param data: the data to push onto the stack
void stack_push(List *stack, uint256_t *val) {
  if (stack_length(stack) > MAX_STACK_DEPTH - 1) {
    char err_msg[50] = "EVM - Stack Overflow\n";
    custom_error(err_msg);
  } else if (stack == NULL) {
    char err_msg[50] = "stack_push: stack is NULL\n";
    custom_error(err_msg);
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

void stack_swap(List *stack, int index) {
  int stack_l = stack_length(stack) - 1;

  if (stack == NULL || index == 0 || index > 15 || index > stack_len) {
    char err_msg[50] = "EVM - Stack element not accessable\n";
    custom_error(err_msg);
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
// peak at the stack at a certain index
// @param stack: stack to peek from
// @param index: index of the stack to peek
// @return stack item
uint256_t stack_peak(List *stack, int index) {
  uint256_t val = init_uint256(0);

  int stack_l = stack_length(stack);

  if (stack == NULL || stack_l == 0 || index > stack_l - 1 || index < -1) {
    // slightly unhelpful error message!
    char err[50] = "EVM - Stack element is not accessable\n";
    custom_error(err);
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

// pop the first element on the stack
// @param stack: stack
void stack_pop(List *stack) {

  if (stack_length(stack) == 0) {
    char err_msg[50] = "EVM - Stack Underflow\n";
    custom_error(err_msg);
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

// gets length of stack
// @param stack: stack to get length of
// @return length of stack
int stack_length(List *stack) {
  if (stack == NULL) {
    char err_msg[50] = "stack_length - Stack is NULL\n";
    custom_error(err_msg);
    return 0;
  } else {
    Node *node = stack->first;
    return stack_len;
  }
}
