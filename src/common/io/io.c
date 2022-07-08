// Ty Scott Kuhl
// https://github.com/skuhl/sys-prog-examples/blob/master/simple-examples/mmap.c

#include "io.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
  ┌───────────────────────────────┐
  │   FILESIZE                    │
  └───────────────────────────────┘
 */

long file_size(const char *filename) {
  long ret;
  FILE *fd = fopen(filename, "r");
  if (!fd) {
    return 0;
  }
  fseek(fd, 0, SEEK_END);
  ret = ftell(fd);
  fclose(fd);
  return ret;
}

/*
  ┌───────────────────────────────┐
  │   READ FILE | MMAP            │
  └───────────────────────────────┘
 */

bool r_file_mmap(char *filename, char *data) {

}

/*
  ┌───────────────────────────────┐
  │   WRITE FILE | MMAP           │
  └───────────────────────────────┘
 */

bool w_file_mmap(char *filename, char *data) {}

/*
  ┌───────────────────────────────┐
  │   READ/WRITE FILE | MMAP      │
  └───────────────────────────────┘
 */

bool rw_file_mmap(char *filename, char *r_data, char *w_data) {}

/*
  ┌───────────────────────────────┐
  │   CREATE FILE                 │
  └───────────────────────────────┘
 */

bool create_file(char *filename, char *data) {
  FILE *fd = fopen(filename, "w");

  fprintf(fd, "%s", data);
  fclose(fd);

  // lol pls fix this
  return true;
}

/*
  ┌───────────────────────────────┐
  │   MMAP ALLOC                  │
  └───────────────────────────────┘
 */

void mmap_alloc(char *filename) {}

/*
  ┌───────────────────────────────┐
  │   PRINTING                    │
  └───────────────────────────────┘
 */

void print_file(const char *filename) {
  // file descriptor
  int fd = open(filename, O_RDONLY);

  size_t mmapLen = file_size(filename);
  off_t offset = 0; // offset to seek to.

  // We use MAP_PRIVATE since writes to the region of memory should
  // not be written back to the file.
  char *ptr = (char *)mmap(NULL, mmapLen, PROT_READ, MAP_PRIVATE, fd, offset);
  if (ptr == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
    // possibly printf empty file error if it occurs?
  }

  // File isn't necessarily loaded into memory until we access the
  // memory (causing a page fault to occur and making the kernel
  // handle it).
  printf("%s\n", ptr);

  close(fd);
}
