#include "client.h"

int setup_TCP_client(const char* host, const char* service) {

    /* ================================================================ */
    /*                Construct foreign address structure               */
    /* ================================================================ */

    struct addrinfo address_criteria;                   /* Criteria for address match */

    memset(&address_criteria, 0, sizeof(address_criteria));

    address_criteria.ai_family = AF_UNSPEC;             /* Any address family */
    address_criteria.ai_socktype = SOCK_DGRAM;          /* Only stream sockets */
    address_criteria.ai_protocol = IPPROTO_UDP;         /* Only TCP protocol */

    struct addrinfo* list;                              /* A list of available addresses */

    /* ================================================================ */


    int result;
    if ((result = getaddrinfo(host, service, NULL, &list)) == 0) {

    }
    else {
        fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(result));
    }

    int client_sock = -1;
    for (struct addrinfo* address = list; address != NULL; address = address->ai_next) {
        if ((client_sock = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) < 0) {
            continue ;
        }

        if (connect(client_sock, address->ai_addr, address->ai_addrlen) == 0) {
            printf("Connected!\n");
            break ;
        }

        close(client_sock);

        client_sock = -1;
    }

    freeaddrinfo(list);

    return client_sock;
}