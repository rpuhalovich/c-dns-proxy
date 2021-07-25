#ifndef QUESTION_H
#define QUESTION_H

#include "headers.h"
#include "utils.h"

#define QUESTION_OFFSET 12

typedef struct {
    unsigned char* qname; // the url who's addres we want to find
    qtype_t qtype; // the type of record that we're recieving (it'll only be AAAA = 28)
    int qclass; // the class we're looking up. (IN, MX etc)

    int qendindex;
} question_t;

question_t* create_question(unsigned char* rawdata);

void free_question(question_t* q);

#endif // QUESTION_H
