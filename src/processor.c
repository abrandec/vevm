
#include "h_stack.h"

#include "uint256.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BYTECODE_LEN 94 // 24KB/256 bits = 93.75 indices, rounded up to 94

void get_opcode(uint256_t *program[], int *pc) {
  // since opcodes are 1 byte, we just use FF and shift it to the right
  // since we're reading the program left to right
  uint64_t *mask;
  *mask = 0xFF00000000000000;
  // get index by dividing by 32
  int index = (*pc / 32);
  int offset = (*pc % 32);


  // offset = *pc % 32 to get position
  // rshift mask with offset
  // then and with program[index]
  printf("INDEX %d\n", index);
  printf("OFFSET %d\n", offset);
  // calculate index of opcode in program
}

void push(List *stack, uint256_t *program[], int *opcode, int *pc) {

  printf("push function\n");
  print_hex_uint256(&program[0]);

  printf("pc %d\n", *pc);

  int bytes2nom = *opcode - 95;
  *pc += bytes2nom;

  printf("bytes2nom: %d\n", bytes2nom);
}

void clear_program(uint256_t *program[]) {

  for (int i = 0; i < MAX_BYTECODE_LEN; ++i) {
    clear_uint256(&program[i]);
  }
}

// print functions

void print_program(uint256_t *program[]) {
  printf("\nPROGRAM\n----------------------------------------------------------"
         "------\n");
  for (int i = 0; i < MAX_BYTECODE_LEN - 1; ++i) {
    print_hex_uint256(&program[i]);
  }
  printf("----------------------------------------------------------------\n");
}

void print_memory(uint256_t memory[]) {
  for (int i = 0; i < MAX_BYTECODE_LEN - 1; ++i) {
    print_hex_uint256(&memory[i]);
  }
}

int vm(void) {
  uint256_t *program[MAX_BYTECODE_LEN];
  uint256_t *testbytecode;

  
  clear_program(program);

  print_program(program);
  // initialize stack
  List *stack = stack_create();
  // initialize program counter
  int pc = 0;
  get_opcode(program, &pc);
  int opcode = 0x61;

  switch (opcode) {
  case 0x00:
    break;

  case 0x60:
  case 0x61:
  case 0x62:
  case 0x63:
  case 0x64:
  case 0x7f:
    printf("push\n");
    push(stack, program, &opcode, &pc);
    printf("pc: %d\n", pc);
    break;
  }
}