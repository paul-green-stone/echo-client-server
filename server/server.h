#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../helper.h"

void serve_TCP_client(int sock);

void send_TCP_hello_msg(int sock);

void log_TCP_request(void);