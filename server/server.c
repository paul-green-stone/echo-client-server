#include <unistd.h>

#include "server.h"

void serve_TCP_client(int sock) {
    /* Buffer for echo string */
    char buffer[BUFSIZ];
    /* Bytes received */
    ssize_t bytes_rcvd = 0;
    ssize_t bytes_sent = 0;


    while (1) {
        if ((bytes_rcvd = read(sock, buffer, BUFSIZ)) == -1) {
            die_with_sys_msg("read() failed");
        }
        else if (bytes_rcvd == 0) {
            die_with_user_msg("read()", "connection closed prematurely");
        }

        printf("%sClient:%s %s\n", ANSI_COLOR_BLUE, ANSI_RESET_ALL, buffer);


        while (bytes_sent < bytes_rcvd) {
            if ((bytes_sent = write(sock, buffer, strlen(buffer))) < 0) {
                die_with_sys_msg("write() failed");
            }
            else if (bytes_sent != strlen(buffer)) {
                die_with_user_msg("write()", "sent unexpected number of bytes");
            }
        }

        memset(buffer, 0, BUFSIZ);

        bytes_rcvd = bytes_sent = 0;
    }

    close(sock);

    return ;
}

int setup_TCP_server(const char* service) {

    /* ================================================================ */
    /*                 Construct local address structure                */
    /* ================================================================ */

    struct addrinfo address_criteria;                   /* Criteria for address match */

    memset(&address_criteria, 0, sizeof(address_criteria));

    address_criteria.ai_family = AF_UNSPEC;             /* Any address family */
    address_criteria.ai_flags = AI_PASSIVE;             /* Accept on any address/port */
    address_criteria.ai_socktype = SOCK_DGRAM;          /* Only stream sockets */
    address_criteria.ai_protocol = IPPROTO_UDP;         /* Only TCP protocol */

    struct addrinfo* list;                              /* A list of available addresses */

    /* ================================================================ */

    int result;
    if ((result = getaddrinfo(NULL, service, NULL, &list)) == 0) {

    }
    else {
        fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(result));
    }

    int server_sock = -1;
    for (struct addrinfo* address = list; address != NULL; address = address->ai_next) {
        if ((server_sock = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) < 0) {
            continue ;
        }

        if (bind(server_sock, address->ai_addr, address->ai_addrlen) == 0) {
            if (listen(server_sock, MAX_PENDING) == 0) {
                /* Print local address of socket */
                struct sockaddr_storage local_address;
                socklen_t address_size = sizeof(local_address);

                if (getsockname(server_sock, (struct sockaddr*) &local_address, &address_size) < 0) {
                    die_with_sys_msg("getsockname() failed");
                }

                printf("Binding to ");

                print_sock_addr((struct sockaddr*) &local_address, stdout);

                printf("\n");

                break ;
            }
        }

        close(server_sock);

        server_sock = -1;
    }

    freeaddrinfo(list);

    return server_sock;
}

int accept_TCP_connection(int server_sock) {
    /* ================================================================ */
    /*                Construct foreign address structure               */
    /* ================================================================ */
    
    struct sockaddr_storage client_address;             /* Client address */
    socklen_t client_length = sizeof(client_address);


    int client_sock = -1;

    if ((client_sock = accept(server_sock, (struct sockaddr*) &client_address, &client_length)) > 0) {
        printf("handling client ");

        print_sock_addr((struct sockaddr*) &client_address, stdout);

        printf("\n");
    }
    else {
        die_with_sys_msg("accept failed");
    }

    return client_sock;
}