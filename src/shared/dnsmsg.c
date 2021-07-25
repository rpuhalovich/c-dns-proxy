#include "dnsmsg.h"

packet_t* create_packet(int fd) {
    packet_t* p = malloc_check(sizeof(packet_t));

    p->size = get_packet_size(fd);
    p->rawdata = malloc_check(sizeof(unsigned char) * p->size);
    read_rep(fd, p->rawdata, p->size);

    return p;
}

packet_t* create_empty_packet() {
    packet_t* p = malloc_check(sizeof(packet_t));

    p->size = 0;
    p->rawdata = NULL;

    return p;
}

packet_t* packet_rcodefourgen(dnsmsg_t* d) {
    packet_t* p = create_empty_packet();

    p->size = d->packet->size;
    p->rawdata = malloc_check(d->packet->size);
    memcpy(p->rawdata, d->packet->rawdata, d->packet->size);

    p->rawdata[2] |= 1UL << 7; // sets the left most bit (bit 7) to 1 (response)
    p->rawdata[3] |= 1UL << 2; // set bit 2 of byte 2 (which will give rcode of 4)
    p->rawdata[3] |= 1UL << 7; // set the ra bit to 1

    return p;
}

void free_packet(packet_t* p) {
    free(p->rawdata);
    free(p);
}

dnsmsg_t* create_dnsmsg(packet_t* p) {
    dnsmsg_t* d = malloc_check(sizeof(dnsmsg_t));

    d->packet = p;
    d->header = create_header(d->packet->rawdata);
    d->question = create_question(&d->packet->rawdata[QUESTION_OFFSET]);
    if(d->header->qr == response) {
        d->answer = create_answer(&d->packet->rawdata[d->question->qendindex + QUESTION_OFFSET]);
    } else {
        d->answer = NULL;
    }

    return d;
}

void free_dnsmsg(dnsmsg_t* d) {
    free_header(d->header);
    free_question(d->question);
    free_answer(d->answer);
    free_packet(d->packet);
    free(d);
}

void dnsmsg_log(dnsmsg_t* d) {
    FILE* fout = fopen_check(LOG_NAME, "a");
    char* ts = get_timestamp();

    if(d->answer != NULL && d->answer->type != AAAA) {
        fprintf(fout, "%s", "");
    } else if(d->question->qtype != AAAA) {
        fprintf(fout, "%s requested %s\n", ts, d->question->qname);
        fflush(fout);
        fprintf(fout, "%s unimplemented request\n", ts);
    } else if (d->header->qr == query) {
        fprintf(fout, "%s requested %s\n", ts, d->question->qname);
    } else if(d->header->qr == response) {
        fprintf(fout, "%s %s is at %s\n", ts, d->question->qname, d->answer->rddata);
    }

    free(ts);
    fflush(fout);
    fclose(fout);
}
