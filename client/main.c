#include "../helper.h"

int main(int argc, char** argv) {
    /* ================================================================ */
    /*                Construct foreign address structure               */
    /* ================================================================ */
    struct sockaddr_in server_addr;                     /* Server address structure */

    in_port_t server_port;                              /* Server port (optional) */

    char buffer[BUFSIZ];                                /* I/O buffer */

    /* Zero out the structure */
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;                   /* IPv4 address family */

    /* Test for correct number of arguments */
    if ( (argc < 2) || (argc > 3) ) {
        die_with_user_msg("Parameter(s)", "<Server Address> [<Server Port>]");
    }

    int result;
    if ((result = inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr)) == 0) {
        die_with_user_msg("inet_pton() failed", "invalid address string");
    }
    else if (result < 0) {
        die_with_sys_msg("socket() failed");
    }

    server_addr.sin_port = htons((argc == 3) ? atoi(argv[2]) : 7);

    /* ================================================================ */


    int client_sock;                                    /* Socket descriptor for client */

        /* Create a reliable, stream socket using TCP */
    if ((client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        die_with_sys_msg("socket() failed");
    }

    /* Establish the connection to the echo server */
    if (connect(client_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        die_with_sys_msg("connect() failed");
    }

    /* ================================================================ */


    size_t bytes_read = 0;                              /* Number of bytes read */
    size_t bytes_written = 0;                           /* Number of bytes written */
    size_t total_bytes_written = 0;                     /* Total number of bytes written */

    while (1) {
        memset(buffer, 0, strlen(buffer));

        printf("%s> %s", ANSI_COLOR_CYAN, ANSI_RESET_ALL);

        /* Take input */
        fgets(buffer, BUFSIZ - 1, stdin);
        /* Add a line termination character */
        buffer[strlen(buffer) - 1] = '\0';

        /* Send a message to the server */
        if ((bytes_read = write(client_sock, buffer, strlen(buffer))) < 0) {
            die_with_sys_msg("write() failed");
        }
        else if (bytes_read != strlen(buffer)) {
            die_with_user_msg("write()", "sent unexpected number of bytes");
        }

        while (total_bytes_written < strlen(buffer)) {
            memset(buffer, 0, strlen(buffer));

            if ((bytes_written = read(client_sock, buffer, BUFSIZ)) == -1) {
                die_with_sys_msg("read() failed");
            }
            else if (bytes_written == 0) {
                die_with_user_msg("read()", "connection closed prematurely");
            }

            /* Keep tally of total bytes */
            total_bytes_written += bytes_written;
            buffer[bytes_written] = '\0';
        }
        
        printf("%sServer:%s %s\n", ANSI_COLOR_BLUE, ANSI_RESET_ALL, buffer);
    }

    close(client_sock);
    
    return EXIT_SUCCESS;
}