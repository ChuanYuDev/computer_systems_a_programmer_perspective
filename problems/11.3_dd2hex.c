#include <arpa/inet.h>
#include <stdio.h>
#include "csapp.h"

int main(int argc, char *argv[])
{
    struct in_addr inaddr;
    
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <dotted decimal string>\n", argv[0]);
        exit(1);
    }

    int rc = inet_pton(AF_INET, argv[1], &inaddr);

    if (rc == 0)
        app_error("inet_pton error: invalid dotted-decimal address");
    else if (rc < 0)
        unix_error("inet_ntop");

    printf("0x%x\n", ntohl(inaddr.s_addr));
    exit(0);
}