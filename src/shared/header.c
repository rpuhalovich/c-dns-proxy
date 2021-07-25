#include "header.h"

header_t* create_header(unsigned char* rawdata) {
    header_t* h = malloc_check(sizeof(header_t));

    h->qr = (read_integer(&rawdata[2], 1) >> 7) & 1U; // checks the left most bit (bit 7)
    h->rcode = (read_integer(&rawdata[3], 1) >> 2) & 1U;
    h->rd = (read_integer(&rawdata[2], 1) >> 0) & 1U;
    h->ra = (read_integer(&rawdata[3], 1) >> 7) & 1U;

    return h;
}

void free_header(header_t* h) {
    free(h);
}
