#ifndef DNSTYPES_H
#define DNSTYPES_H

#define LOG_NAME "dns_svr.log"

typedef enum { AAAA = 28 } qtype_t;
typedef enum { query = 0, response = 1 } QR;
typedef enum { squery = 0, iquery = 1, ssrequest = 2 } Opcode;

#endif // DNSTYPES_H
