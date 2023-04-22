#include "helper.h"

void die_with_user_msg(const char* msg, const char* details) {
    fprintf(stderr, "%s%s%s: %s\n", ANSI_COLOR_RED, msg, ANSI_RESET_ALL, details);

    exit(1);
}

void die_with_sys_msg(const char* msg) {
    fprintf(stderr, "%s%s%s: %s\n", ANSI_COLOR_RED, msg, ANSI_RESET_ALL, strerror(errno));

    exit(1);
}

void print_sock_addr(const struct sockaddr* address, FILE* stream) {
    void* numeric_addr = NULL;                      /* Pointer to binary address */
    char addr_buffer[INET6_ADDRSTRLEN];             /* Buffer to contain result */
    in_port_t port;

    if ((address != NULL) && (stream != NULL)) {
        switch (address->sa_family) {
            case AF_INET:
                numeric_addr = &((struct sockaddr_in*) address)->sin_addr;
                port = ntohs(((struct sockaddr_in*) address)->sin_port);

                break ;

            case AF_INET6:
                numeric_addr = &((struct sockaddr_in6*) address)->sin6_addr;
                port = ntohs(((struct sockaddr_in6*) address)->sin6_port);

                break ;

            default:
                fprintf(stream, "[unknown type]");

                return ;
        }
    }

    /* Convert binary to printable address */
    if (inet_ntop(address->sa_family, numeric_addr, addr_buffer, INET6_ADDRSTRLEN) == NULL) {
        fprintf(stderr, "[invalid address]");
    }
    else {
        fprintf(stream, "%s", addr_buffer);

        if (port != 0) {
            fprintf(stream, "-%u", port);
        }
    }

    return ;
}