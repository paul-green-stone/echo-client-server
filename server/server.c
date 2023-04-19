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