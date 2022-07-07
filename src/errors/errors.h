#ifndef ERRORS_H
#define ERRORS_H

#include <inttypes.h>

/*
  ┌───────────────────────────────┐
  │   FUNCTIONS                   │
  └───────────────────────────────┘
 */

// custom error handling
// @param err_code: error code
void custom_error(uint8_t err_code);

#endif