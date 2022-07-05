#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

void custom_error(char err_msg[]) {
  printf("%s\n", err_msg);
  exit(1);
}