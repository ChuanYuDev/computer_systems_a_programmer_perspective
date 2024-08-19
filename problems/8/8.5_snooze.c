#include <stdio.h>
#include <unistd.h>

unsigned int snooze(unsigned int secs)
{
    unsigned int left_secs = sleep(secs);
    printf("Slept for %d of %d secs.\n", secs - left_secs, secs);
    return left_secs;
}

int main(int argc, char **argv, char **envp)
{
    snooze(2);
    return 0;
}
