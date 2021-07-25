#include "utils.h"

void* malloc_check(size_t n) {
    assert(n > 0);
    void* p = malloc(n);
    assert(p);
    return p;
}

FILE* fopen_check(const char* fname, const char* mode) {
    FILE* f = fopen(fname, mode);
    assert(f);
    return f;
}

// code modified from: https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
char* get_timestamp() {
    // example: 2021-04-24T05:12:32+0000
    time_t rawtime;
    struct tm *info;
    char* buffer = malloc_check(sizeof(char) * TSLEN);
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, TSLEN, "%FT%T%z", info);
    return buffer;
}

int get_packet_size(int stream) {
    int num_bytes = 2;
    unsigned char* buff = malloc_check(sizeof(unsigned char) * num_bytes);
    read_rep(stream, buff, num_bytes);
    int out = read_integer(buff, num_bytes);
    free(buff);
    return out;
}

int read_integer(unsigned char* rawdata, int n) {
    unsigned char* tmp_raw = malloc_check(sizeof(unsigned char) * n);
    int out_int;

    memcpy(tmp_raw, rawdata, n);
    if(n == 1) {
        uint8_t* tmp_num = malloc_check(sizeof(uint8_t));
        memcpy(tmp_num, tmp_raw, n);
        out_int = *tmp_num;
        free(tmp_num);
    } else if(n == 2) {
        uint16_t* tmp_num = malloc_check(sizeof(uint16_t));
        memcpy(tmp_num, tmp_raw, n);
        out_int = (int)ntohs(*tmp_num);
        free(tmp_num);
    } else if(n == 4) {
        uint32_t* tmp_num = malloc_check(sizeof(uint32_t));
        memcpy(tmp_num, tmp_raw, n);
        out_int = (int)ntohl(*tmp_num);
        free(tmp_num);
    } else {
        free(tmp_raw);
        exit(EXIT_FAILURE);
    }
    free(tmp_raw);

    return out_int;
}

void error_exit(char* m, int status) {
    printf("%s\n", m);
    exit(status);
}

void perror_exit(char* m) {
    perror(m);
    exit(EXIT_FAILURE);
}

void write_rep(int fd, unsigned char* buf, size_t n) {
    // keep writing to fd until it actually takes all bytes of data
    int total = 0, m;
    do {
        if((m = write(fd, &buf[total], (n - total))) < 0) perror_exit("writing to fd");
        total += m;
    } while(total != n);
}

void read_rep(int fd, unsigned char* buf, size_t n) {
    // keep reading from fd until it actually takes all bytes of data
    // read from the buffer index that you've left off at
    int total = 0, m;
    do {
        if((m = read(fd, &buf[total], (n - total))) < 0) perror_exit("reading from fd");
        total += m;
    } while(total != n);
}
