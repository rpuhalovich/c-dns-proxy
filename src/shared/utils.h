#ifndef UTILS_H
#define UTILS_H

#include "headers.h"

// length of the time stamp including the null terminator
#define TSLEN 25

/**
 * \brief malloc that automatically asserts
 */
void* malloc_check(size_t n);

/**
 * \brief fopen that automatically asserts
 */
FILE* fopen_check(const char* fname, const char* mode);

/**
 * \brief returns a malloced string that has the specified timestamp formatting
 */
char* get_timestamp();

/**
 * \brief reads tcp header from stream
 * \return returns the size of the packet in MB (information that's within the first two bytes of the packet)
 */
int get_packet_size(int stream);

/**
 * \brief reads the integer value from n bytes of binary network memory
 * \param rawdata pointer to location from which to start reading n bytes from
 * \param n number of bytes to be read in
 * \return returns the integer that is read from n bytes
 */
int read_integer(unsigned char* rawdata, int n);

/**
 * \brief prints m to stdout and exits with EXIT_FAILURE
 */
void error_exit(char* m, int status);

/**
 * \brief for the perror used in the spec
 */
void perror_exit(char* m);

/**
 * \brief write until all bytes are sent. also automatically checks for errors
 */
void write_rep(int fd, unsigned char* buf, size_t n);

/**
 * \brief read until all bytes are recieved. also automatically checks for errors
 */
void read_rep(int fd, unsigned char* buf, size_t n);

#endif // UTILS_H
