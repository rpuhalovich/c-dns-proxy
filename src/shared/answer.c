#include "answer.h"

answer_t* create_answer(unsigned char* rawdata) {
    answer_t* a = malloc_check(sizeof(answer_t));

    a->name = (uint16_t)read_integer(rawdata, 2);

    a->type = read_integer(&rawdata[2], 2);
    // we only want answers of type AAAA
    if(a->type != AAAA) {
        a->class = 0;
        a->ttl = 0;
        a->rdlength = 0;
        a->rddata = NULL;
        return a;
    }

    a->class = read_integer(&rawdata[4], 2);
    a->ttl = read_integer(&rawdata[6], 4);
    a->rdlength = (uint16_t)read_integer(&rawdata[10], 2);

    // reading in rddata which has length of rdlength
    unsigned char* tmp = malloc_check(sizeof(unsigned char) * (size_t)a->rdlength);
    memcpy(tmp, &rawdata[12], (size_t)a->rdlength);
    a->rddata = malloc_check(sizeof(char) * INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, tmp, a->rddata, INET6_ADDRSTRLEN);
    free(tmp);

    return a;
}

void free_answer(answer_t* a) {
    if(a == NULL) return;
    if(a->rddata != NULL) free(a->rddata);
    free(a);
}
