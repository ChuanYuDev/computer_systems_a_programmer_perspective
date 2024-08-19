#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void sigint_handler(int sig)
{
    return;
}

unsigned int snooze(unsigned int secs)
{
    unsigned int left_secs = sleep(secs);
    printf("Slept for %d of %d secs.\n", secs - left_secs, secs);

    return left_secs;
}

int main(int argc, char *argv[], char *envp[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <secs>\n", argv[0]);
        exit(0);
    }


    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        unix_error("signal error");

    // long int secs = strtol(argv[1], NULL, 10);
    snooze(atoi(argv[1]));

    // return 0;
    exit(0);
}