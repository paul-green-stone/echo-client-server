#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../helper.h"

int main(int argc, char** argv) {
    /* Server IP address (dotted quad) */
    char* server_IP = NULL;
    /* I/O buffer */
    char buffer[BUFSIZ];
    /* Server port (optional). 7 is a well-known echo port */
    in_port_t server_port;
    /* */
    int sock;
    /* Server address structure */
    struct sockaddr_in6 server_addr;
    /* Address conversion status */
    int rtn_val;
    /* Number of bytes sent and received */
    size_t bytes_sent = 0;
    size_t bytes_rcvd = 0;
    size_t total_bytes_rcvd = 0;



    /* Test for correct number of arguments */
    if ( (argc < 2) || (argc > 3) ) {
        die_with_user_msg("Parameter(s)", "<Server Address> [<Server Port>]");
    }

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        die_with_sys_msg("socket() failed");
    }


    server_IP = argv[1];
    server_port = (argc == 3) ? atoi(argv[2]) : 7;

    /* Zero out the structure */
    memset(&server_addr, 0, sizeof(server_addr));
    /* IPv4 address family */
    server_addr.sin6_family = AF_INET6;


    /* Convert address */
    rtn_val = inet_pton(AF_INET6, server_IP, &server_addr.sin6_addr.s6_addr);

    if (rtn_val == 0) {
        die_with_user_msg("inet_pton() failed", "invalid address string");
    }
    else if (rtn_val < 0) {
        die_with_sys_msg("socket() failed");
    }

    server_addr.sin6_port = htons(server_port);


    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        die_with_sys_msg("connect() failed");
    }


    while (1) {
        memset(buffer, 0, strlen(buffer));

        printf("%s> %s", ANSI_COLOR_CYAN, ANSI_RESET_ALL);

        /* Take input */
        fgets(buffer, BUFSIZ - 1, stdin);
        /* Add a line termination character */
        buffer[strlen(buffer) - 1] = '\0';

        /* Send a message to the server */
        if ((bytes_sent = write(sock, buffer, strlen(buffer))) < 0) {
            die_with_sys_msg("write() failed");
        }
        else if (bytes_sent != strlen(buffer)) {
            die_with_user_msg("write()", "sent unexpected number of bytes");
        }

        while (total_bytes_rcvd < strlen(buffer)) {
            memset(buffer, 0, strlen(buffer));

            if ((bytes_rcvd = read(sock, buffer, BUFSIZ)) == -1) {
                die_with_sys_msg("read() failed");
            }
            else if (bytes_rcvd == 0) {
                die_with_user_msg("read()", "connection closed prematurely");
            }

            /* Keep tally of total bytes */
            total_bytes_rcvd += bytes_rcvd;
            buffer[bytes_rcvd] = '\0';
        }
        
        printf("%sServer:%s %s\n", ANSI_COLOR_BLUE, ANSI_RESET_ALL, buffer);
    }

    close(sock);
    exit(0);
}