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
    /* String to echo */
    char* echo_str = NULL;
    /* I/O buffer */
    char buffer[BUFSIZ];
    /* Echo string length */
    size_t echo_str_len;
    /* Server port (optional). 7 is a well-known echo port */
    in_port_t server_port;
    /* */
    int sock;
    /* Server address structure */
    struct sockaddr_in server_addr;
    /* Address conversion status */
    int rtn_val;
    /* Number of bytes sent and received */
    size_t bytes_sent = 0;
    size_t bytes_rcvd = 0;
    size_t total_bytes_rcvd = 0;



    /* Test for correct number of arguments */
    if ( (argc < 3) || (argc > 4) ) {
        die_with_user_msg("Parameter(s)", "<Server Address> <Echo Word> [<Server Port>]");
    }

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        die_with_sys_msg("socket() failed");
    }


    server_IP = argv[1];
    echo_str = argv[2];
    server_port = (argc == 4) ? atoi(argv[3]) : 7;

    /* Zero out the structure */
    memset(&server_addr, 0, sizeof(server_addr));
    /* IPv4 address family */
    server_addr.sin_family = AF_INET;


    /* Convert address */
    rtn_val = inet_pton(AF_INET, server_IP, &server_addr.sin_addr.s_addr);

    if (rtn_val == 0) {
        die_with_user_msg("inet_pton() failed", "invalid address string");
    }
    else if (rtn_val < 0) {
        die_with_sys_msg("socket() failed");
    }

    server_addr.sin_port = htons(server_port);


    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        die_with_sys_msg("connect() failed");
    }

    /* Determine input length */
    echo_str_len = strlen(echo_str);


    /* Send the string to the server */
    if ((bytes_sent = send(sock, echo_str, echo_str_len, 0)) < 0) {
        die_with_sys_msg("send() failed");
    }
    else if (bytes_sent != echo_str_len) {
        die_with_user_msg("send()", "sent unexpected number of bytes");
    }


    /* Receive the same string back from the server */
    printf("Received: ");

    while (total_bytes_rcvd < echo_str_len) {
        if ((bytes_rcvd = recv(sock, buffer, BUFSIZ - 1, 0)) < 0) {
            die_with_sys_msg("recv() failed");
        }
        else if (bytes_rcvd == 0) {
            die_with_user_msg("recv()", "connection closed prematurely");
        }

        /* Keep tally of total bytes */
        total_bytes_rcvd += bytes_rcvd;
        buffer[bytes_rcvd] = '\0';

        printf("%s", buffer);
    }

    printf("\n");

    close(sock);
    exit(0);
}