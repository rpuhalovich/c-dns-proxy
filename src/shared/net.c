#include "net.h"

client_t* create_acceptfd(int listenfd) {
    client_t* c = malloc(sizeof(client_t));
    memset(c, 0, sizeof(client_t));
    c->acceptfd = accept(listenfd, (struct sockaddr*)&c->connectaddr, &c->connectaddr_size);
    if(c->acceptfd < 0) perror_exit("accept");
    return c;
}

void free_client(client_t* c) {
    close(c->acceptfd);
    free(c);
}

int create_listenfd(char* listen_port) {
    struct addrinfo hints, *res;

    // setup address struct
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6; // use IPv6
    hints.ai_socktype = SOCK_STREAM; // socket stream
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    if(getaddrinfo(NULL, listen_port, &hints, &res) != 0) error_exit("getaddrinfo error.", EXIT_FAILURE);

    int listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(listenfd < 0) perror_exit("listen");

    int enable = 1; // as required by spec
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) perror_exit("setsockopt");
    if(bind(listenfd, res->ai_addr, res->ai_addrlen) < 0) perror_exit("bind"); // bind address to the socket
    freeaddrinfo(res);

    return listenfd;
}

int create_upstreamfd(char* ip, char* port) {
    int upstreamfd;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(ip, port, &hints, &res) != 0) error_exit("getaddrinfo error.", EXIT_FAILURE);

    // connect to the first valid response
    struct addrinfo* rp;
    for(rp = res; rp != NULL; rp = rp->ai_next) {
        upstreamfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(upstreamfd == -1) continue; // fail
        if(connect(upstreamfd, rp->ai_addr, rp->ai_addrlen) != -1) break; // success
        close(upstreamfd);
    }
    if(rp == NULL) error_exit("client failed to connect.", EXIT_FAILURE);
    freeaddrinfo(res);

    return upstreamfd;
}

void net_listen(int listenfd, int backlog) {
    // listen to the socket with a queue of backlog
    if(listen(listenfd, backlog) < 0) perror_exit("listening to socket");
}

void net_write(int fd, dnsmsg_t* d) {
    // @793 - According to this post, you need to reconstruct the two byte header when sending to the upstream.
    uint16_t b = htons(d->packet->size);
    write_rep(fd, (unsigned char*)&b, sizeof(b)); // two byte header
    write_rep(fd, d->packet->rawdata, d->packet->size); // dns packet
}
