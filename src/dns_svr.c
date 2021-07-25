#include "headers.h"
#include "dnsmsg.h"
#include "net.h"

#define LISTENPORT "8053"
#define BACKLOG 10

/**
 * --- Notes ---
 * Order of events:
 * 1. Setup listen socket, accept client connection - in this case a TCP connection from any "AAAA" connection.
 * 2. Read the packet and log it to dns_svr.log.
 * 3. Forward the packet to the upstream server socket that you get via the args.
 * 4. Read the response from the upstream server socket and log it to dns_svr.log.
 * 5. Send the response back to the client over the same TCP connection.
 * 
 * Note that there is a test that has bytes held back to be sent over multiple reads.
 * 
 * @793 - According to this post, you need to reconstruct the two byte header when sending to the upstream.
 * 
 * @744 - If the type of query sent by the client is not AAAA, do not send to the upstream, instead respond with an
 * rcode = 4.
 * - Change the QR bit to response.
 * - Change the RCODE to 4.
 * 
 * // TODO: update the qname reading to include other special characters if needed.
 */

int main(int argc, char* argv[]) {
    if(argc != 3) error_exit("Usage: ./dns_svr <IPv4 Address> <Port>", EXIT_FAILURE);
    remove(LOG_NAME);

    dnsmsg_t* d;

    int listenfd = create_listenfd(LISTENPORT);
    net_listen(listenfd, BACKLOG);

    for(;;) {
        // get socket to client
        client_t* c = create_acceptfd(listenfd);
        d = create_dnsmsg(create_packet(c->acceptfd));
        dnsmsg_log(d);

        if(d->question->qtype != AAAA) {
            // generate dnsmsg with rcode 4 and send back to client on c->acceptfd
            dnsmsg_t* drcodefour = create_dnsmsg(packet_rcodefourgen(d));
            net_write(c->acceptfd, drcodefour);
            free_dnsmsg(drcodefour);
        } else {
            // forward to server
            int upstreamfd = create_upstreamfd(argv[1], argv[2]);
            net_write(upstreamfd, d);
            free_dnsmsg(d);

            // read message from server
            d = create_dnsmsg(create_packet(upstreamfd));
            dnsmsg_log(d);

            // write message to the same TCP client that sent the initial query
            net_write(c->acceptfd, d);
            close(upstreamfd);
        }

        free_dnsmsg(d);
        free_client(c); // also closes c->acceptfd
    }

    close(listenfd);
    return 0;
}
