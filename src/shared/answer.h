#ifndef ANSWER_H
#define ANSWER_H

#include "headers.h"
#include "utils.h"

typedef struct {
    /*
     * This is the offset from the start of the DNS message that gets you to the URL
     * that is in the question section of the DNS message. For compression purposes.
     */
    uint16_t name;

    qtype_t type; // the type of record that we're recieving (it'll only be AAAA = 28)
    int class; // the class we're looking up. (IN, MX etc)
    uint32_t ttl; // time to live for this response in seconds
    uint16_t rdlength; // length of the RDDATA section
    char* rddata; // the data for the IP address (4 bytes)
} answer_t;

answer_t* create_answer(unsigned char* rawdata);

void free_answer(answer_t* a);

#endif // ANSWER_H
