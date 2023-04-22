#include "../helper.h"
#include "server.h"

/* Maximum outstanding connection requests */
#define MAX_PENDING 5

int main(int argc, char** argv) {

    /* ================================================================ */
    /*                Construct local address structure                 */
    /* ================================================================ */
    struct sockaddr_in server_addr;                         /* Local address */

    /* Zero out the structure */
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;                       /* IPv4 address family */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);        /* Any incoming interface */

    /* Test for correct number of arguments */
    if (argc != 2) {
        die_with_user_msg("Parameter(s)", "<Server Port>");
    }

    server_addr.sin_port = htons(atoi(argv[1]));            /* Local port */
    /* ================================================================ */


    int server_sock;                                        /* Socket descriptor for server */

    /* Create a socket for incoming connections */
    if ((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        die_with_sys_msg("socket() failed");
    }


    /* Bind to the local port */
    if (bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        die_with_sys_msg("bind() failed");
    }


    /* Mark the socket so it will listen for incoming connections */
    if (listen(server_sock, MAX_PENDING) < 0) {
        die_with_sys_msg("listen() failed");
    }


    /* ================================================================ */
    /*                   Wait for a client to connect                   */
    /* ================================================================ */
    for (; ;) {
        struct sockaddr_in client_addr;                     /* Client address */
        socklen_t client_addr_len = sizeof(client_addr);    /* Length of client address structure */    
        int client_sock;                                    /* Client socket */
        char client_name[INET_ADDRSTRLEN];                  /* String to contain client address */


        /* Wait for a client to connect */
        if ((client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &client_addr_len)) < 0) {
            die_with_sys_msg("accept() failed");
        }


        if (inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_name, sizeof(client_name)) != NULL) {
            printf("%sHandling client%s %s/%d\n", ANSI_COLOR_GREEN, ANSI_RESET_ALL, client_name, ntohs(client_addr.sin_port));
        }
        else {
            printf("%sUnable to get client address.%s\n", ANSI_COLOR_RED, ANSI_RESET_ALL);
        }

        serve_TCP_client(client_sock);
    }
}