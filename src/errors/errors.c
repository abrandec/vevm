#include "errors.h"
#include "../core/stack/stack.h"

#include <stdio.h>
#include <stdlib.h>

void custom_error(const char err_msg[]) {
  #ifdef TEST_MODE
  
  #else
  printf("%s\n", err_msg);
  exit(1);
  #endif
}

void critical_error(const char err_msg[]) {
  printf("%s\n", err_msg);
  exit(1);
}