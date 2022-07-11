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

  folder_exists(fd, "test_data") ? 0 : create_folder("test_data");
  create_file(fd, "test_data/test_file.txt", "test data");
  long file_size;
  data = read_file_fmmap(fd, "test_data/test_file.txt", &file_size);

  return assert_msg("Read check", assert_char("test data", data));
}
bool io_test_write_fmmap(void) { return true; }

bool io_tests(void) {
  // number of tests
  enum { test_len = 2 };

  bool test_results[test_len] = {io_test_read_fmmap(), io_test_write_fmmap()};

  // check if all test results passed
  return assert_bool_array_msg("IO results", test_results, test_len);
}