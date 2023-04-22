#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "../helper.h"

int main(int argc, char** argv) {

    /* Test for correct number of arguments */
    if (argc != 2) {
        die_with_user_msg("Parameter(s)", "<Server Port>");
    }

    /* ================================================================ */


    int server_sock;                                            /* Socket descriptor for server */

    if ((server_sock = setup_TCP_server(argv[1])) == -1) {
        die_with_user_msg("SetupTCPServerSocket() failed", argv[1]);
    }

    /* ================================================================ */


    /* Wait for a client to connect */
    for (; ;) {
        int client_sock;                                        /* Client socket */

        if ((client_sock = accept_TCP_connection(server_sock)) > 0) {
            serve_TCP_client(client_sock);
        }

        close(client_sock);
    }

    close(server_sock);

    return EXIT_SUCCESS;
}