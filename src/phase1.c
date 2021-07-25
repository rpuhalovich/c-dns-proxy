#include "headers.h"
#include "dnsmsg.h"

int main(int argc, char* argv[]) {
    if (argc != 2) error_exit("Usage: ./phase1 [query|response] < <path-to-packet-file>", EXIT_FAILURE);
    remove(LOG_NAME);

    dnsmsg_t* d = create_dnsmsg(create_packet(STDIN_FILENO));
    dnsmsg_log(d);
    free_dnsmsg(d);

    fflush(stdout);
    return 0;
}
