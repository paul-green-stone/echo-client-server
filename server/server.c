#include <unistd.h>

#include "server.h"

void serve_TCP_client(int sock) {
    /* Buffer for echo string */
    char buffer[BUFSIZ];
    /* Bytes received */
    ssize_t bytes_rcvd;
    ssize_t bytes_sent;


    /* Receive message from client */
    if ((bytes_rcvd = recv(sock, buffer, BUFSIZ, 0)) < 0) {
        die_with_sys_msg("recv() failed");
    }


    /* Send received string and receive again until end of stream */
    while (bytes_rcvd > 0) {
        if ((bytes_sent = send(sock, buffer, bytes_rcvd, 0)) < 0) {
            die_with_sys_msg("send() failed");
        }
        else if (bytes_sent != bytes_rcvd) {
            die_with_user_msg("send()", "sent unexpected number of bytes");
        }


        /* See if there is more data to receive */
        if ((bytes_rcvd = recv(sock, buffer, BUFSIZ, 0)) < 0) {
            die_with_sys_msg("recv() failed");
        }
    }

    close(sock);

    return ;
}