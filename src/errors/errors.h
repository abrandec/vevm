#ifndef ERRORS_H
#define ERRORS_H

/*
  ┌────────────────────────────────────────────────────────────────────────────┐
  │                                                                            │
  │   FUNCTIONS                                                                │
  │                                                                            │
  └────────────────────────────────────────────────────────────────────────────┘
 */

// custom error handling
// @param err_msg: error message
void custom_error(const char err_msg[]);

// critical error handling
// @param err_msg: error message
void critical_error(const char err_msg[]);

#endif