#ifndef HEADER_H
#define HEADER_H

#include "headers.h"
#include "utils.h"

#define IDLEN 2

typedef struct {
    QR qr; // query (false) or response (true)
    unsigned int rcode;
    bool rd; // recursive desired
    bool ra; // recursive available
} header_t;

header_t* create_header(unsigned char* rawdata);

void free_header(header_t* h);

#endif // HEADER_H
