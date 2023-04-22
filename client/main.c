#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../helper.h"
#include "client.h"

int main(int argc, char** argv) {

    /* ================================================================ */
    /*                Construct foreign address structure               */
    /* ================================================================ */

    /* Test for correct number of arguments */
    if ( (argc < 2) || (argc > 3) ) {
        die_with_user_msg("Parameter(s)", "<Server Address> [<Server Port>]");
    }

    char buffer[BUFSIZ];                                /* I/O buffer */

    /* ================================================================ */


    int client_sock;                                    /* Socket descriptor for client */
    
    if ((client_sock = setup_TCP_client(argv[1], argv[2])) < 0) {
        die_with_user_msg("SetupTCPClientSocket() failed", "unable to connect");
    }

    printf("%d\n", client_sock);

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