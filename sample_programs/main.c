#include "../src/h_stack.h"
#include "../src/processor.c"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  static uint256_t program[MAX_BYTECODE_LEN];

  bool DEBUG = false;
  int i;

  for (i = 0; i < argc; ++i) {

    // debug flag
    if (strcmp(argv[i], "-DEBUG") == 0 || strcmp(argv[i], "-debug") == 0 ||
        strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "-d") == 0) {
      DEBUG = true;
    
    // help flag
    } else if (strncmp(argv[i], "-HELP", 1) == 0 ||
               strncmp(argv[i], "-help", 1) == 0 ||
               strncmp(argv[i], "-H", 1) == 0 ||
               strncmp(argv[i], "-h", 1) == 0) {
      
      // Title + logo           
      printf("Vision EVM Options      \033[93m▓▓\033[94m▓▓\033[92m▓▓\033[35m▓▓\033[91m▓▓\033[00m\n");
      printf("──────────────────────────────────\n");
      // Options
      printf("-debug, -d : debug mode\n"
             "-help,  -h : print this help msg\n");
      
      exit(0);
    }
  }

  vm(program, &DEBUG);
  return 0;
}