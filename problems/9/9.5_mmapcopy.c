#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>

// void *Mmap()

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void *Mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)
{
    void *ptr;

    if ((ptr = mmap(start, length, prot, flags, fd, offset)) == ((void *) -1))
        unix_error("mmap error");
    
    return ptr;
}

void Munmap(void *start, size_t length)
{
    if (munmap(start, length) < 0)
        unix_error("munmap error");
}