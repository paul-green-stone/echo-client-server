#include "helper.h"

void die_with_user_msg(const char* msg, const char* details) {
    fprintf(stderr, "%s%s%s: %s\n", ANSI_COLOR_RED, msg, ANSI_RESET_ALL, details);

    exit(1);
}

void die_with_sys_msg(const char* msg) {
    fprintf(stderr, "%s%s%s: %s\n", ANSI_COLOR_RED, msg, ANSI_RESET_ALL, strerror(errno));
}
