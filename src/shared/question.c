#include "question.h"

question_t* create_question(unsigned char* rawdata) {
    question_t* q = (question_t*)malloc_check(sizeof(question_t));

    // construct qname (domain name)
    int offset = 0;
    do {
        int len = read_integer(&rawdata[offset], 1);
        offset += (len + 1);
    } while(rawdata[offset] != 0);

    q->qname = malloc_check(sizeof(unsigned char) * offset);
    memcpy(q->qname, &rawdata[1], offset);
    for(int i = 0; i < offset; i++) {
        if(!isalpha(q->qname[i]) && !isdigit(q->qname[i]) && q->qname[i] != '-') q->qname[i] = '.';
    }
    q->qname[offset - 1] = '\0';

    q->qtype = read_integer(&rawdata[offset + 1], 2);
    q->qclass = read_integer(&rawdata[offset + 3], 2);
    q->qendindex = offset + 5;

    return q;
}

void free_question(question_t* q) {
    free(q->qname);
    free(q);
}
