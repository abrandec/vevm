#ifndef IO_H
#define IO_H

#include <stdbool.h>

/*
  ┌───────────────────────────────┐
  │   FILESIZE                    │
  └───────────────────────────────┘
 */

// get the size of a file
// @param filename: the file to get the size of
// @return the size of the file
long file_size(const char *filename);

/*
  ┌───────────────────────────────┐
  │   READ FILE | MMAP            │
  └───────────────────────────────┘
 */

// read a file using mmap
// @param filename: the file to read
// @param data: the return data to read from the file
// @return true if successful, false otherwise
bool r_file_mmap(char *filename, char *data);

/*
  ┌───────────────────────────────┐
  │   WRITE FILE | MMAP           │
  └───────────────────────────────┘
 */

// write to a file using mmap
// @param filename: the file to write to
// @param data: the data to write to the file
// @return true if successful, false otherwise
bool w_file_mmap(char *filename, char *data);

/*
  ┌───────────────────────────────┐
  │   READ/WRITE FILE | MMAP      │
  └───────────────────────────────┘
 */

// read and write to a file using mmap
// @param filename: the file to read and write to
// @param r_data: the data to read from the file
// @param w_data: data to write to the file
// @return true if successful, false otherwise
bool rw_file_mmap(char *filename, char *r_data, char *w_data);

/*
  ┌───────────────────────────────┐
  │   CREATE FILE                 │
  └───────────────────────────────┘
 */

// create a file
// @param filename: the file to create
// @param size: the size of the file to create
// @param data: the data to write to the file
// @return true if successful, false otherwise
bool create_file(char *filename, char *data);

/*
  ┌───────────────────────────────┐
  │   MMAP ALLOC                  │
  └───────────────────────────────┘
 */

// allocate memory using mmap
// @param filename: the file to allocate memory for
void mmap_alloc(char *filename);

/*
  ┌───────────────────────────────┐
  │   PRINTING                    │
  └───────────────────────────────┘
 */

// print a file
// @param filename: the file to print
void print_file(const char *filename);

#endif