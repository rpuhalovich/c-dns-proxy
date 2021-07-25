#ifndef NET_H
#define NET_H

#include "headers.h"
#include "utils.h"
#include "dnsmsg.h"

typedef struct {
    struct sockaddr_storage connectaddr; // stores information about the client connection
    socklen_t connectaddr_size;
    int acceptfd;
} client_t;

/**
 * \brief takes in the listenfd and accepts the connection to it
 * \return client_t structut that contains the acceptfd, as well as client info
 * 
 * The acceptfd is also used to write back to the client that originally created this fd.
 */
client_t* create_acceptfd(int listenfd);

void free_client(client_t* c);

/**
 * \brief creates and binds the listenfd
 * \param listen_port the string representation of the port to listen on
 * \return listenfd
 */
int create_listenfd(char* listen_port);

/**
 * \return fd to the upstream
 */
int create_upstreamfd(char* ip, char* port);

/**
 * \brief listens to the listenfd with a backlog of size backlog. exits with error if listen returns < 0
 */
void net_listen(int listenfd, int backlog);

/**
 * \brief writes to the given fd
 */
void net_write(int fd, dnsmsg_t* d);

#endif // NET_H
