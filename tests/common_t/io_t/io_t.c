#include "io_t.h"

#include "../../../src/common/cmd/cmd.h"
#include "../../../src/common/io/io.h"

#include "../../test_utils/assert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool io_test_read_fmmap(void) {
  FILE *fd;
  char *data;

  const char cannot_read_f[20] = "can't read file\0";

  
  create_file(fd, "test_data/test_file.txt", "test data");
  long file_size;
  data = read_file_fmmap(fd, "test_data/test_file.txt", &file_size);

  return assert_char(__func__, cannot_read_f, "test data", data);
}

bool io_test_write_fmmap(void) { return true; }

bool io_tests(void) {
  // number of tests
  enum { test_len = 1 };

  bool test_results[test_len] = {
      io_test_read_fmmap() //, io_test_write_fmmap()
  };

  // check if all test results passed
  return assert_bool_array_msg("IO TESTS", test_results, test_len);
}