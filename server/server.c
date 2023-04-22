#include <unistd.h>

#include "server.h"

void serve_TCP_client(int sock) {
    char buffer[BUFSIZ];                            /* Buffer for echo string */
    ssize_t bytes_read = 0;                         /* Bytes received */
    ssize_t bytes_written = 0;                      /* Bytes sent */


    while (1) {
        if ((bytes_read = read(sock, buffer, BUFSIZ)) == -1) {
            die_with_sys_msg("read() failed");
        }
        else if (bytes_read == 0) {
            die_with_user_msg("read()", "connection closed prematurely");
        }

        printf("%sClient:%s %s\n", ANSI_COLOR_BLUE, ANSI_RESET_ALL, buffer);


        while (bytes_written < bytes_read) {
            if ((bytes_written = write(sock, buffer, strlen(buffer))) < 0) {
                die_with_sys_msg("write() failed");
            }
            else if (bytes_written != strlen(buffer)) {
                die_with_user_msg("write()", "sent unexpected number of bytes");
            }
        }

        memset(buffer, 0, BUFSIZ);

        bytes_read = bytes_written = 0;
    }

    close(sock);

    return ;
}