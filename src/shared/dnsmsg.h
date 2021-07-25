#ifndef DNSMSG_H
#define DNSMSG_H

#include "headers.h"
#include "utils.h"

#include "header.h"
#include "question.h"
#include "answer.h"

/**
 * This is purely used to read data into a dnsmsg struct.
 * Won't need to free rawdata as freeing the dnsmsg would do that for us.
 */
typedef struct {
    int size;
    unsigned char* rawdata;
} packet_t;

typedef struct {
    packet_t* packet;
    header_t* header;
    question_t* question;
    answer_t* answer;
} dnsmsg_t;

// --- packet_t ---

packet_t* create_packet(int fd);

packet_t* create_empty_packet();

/**
 * \brief generates a dns response with rcode 4 and ra == true (1)
 * \return a pointer to a new packet_t that contains the rawdata and size for a response of rcode 4
 */
packet_t* packet_rcodefourgen(dnsmsg_t* d);

void free_packet(packet_t* p);

// --- dnsmsg_t ---

dnsmsg_t* create_dnsmsg(packet_t* p);

void free_dnsmsg(dnsmsg_t* d);

/**
 * \brief prints the inputted msg with a timestamp at the front to the dns_svr.log file
 */
void dnsmsg_log(dnsmsg_t* d);

#endif // DNSMSG_H
