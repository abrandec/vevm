#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

void custom_error(const char err_msg[]) {
  printf("%s\n", err_msg);
  exit(1);
}

void critical_error(const char err_msg[]) {
  printf("%s\n", err_msg);
  exit(1);
}