#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

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

int Fstat(int fd, struct stat *buf)
{
    int rc;

    if ((rc = fstat(fd, buf)) == -1)
        unix_error("fstat error");

    return rc;
}

int Open(char *filename, int flags, mode_t mode)
{
    int fd;

    if ((fd = open(filename, flags, mode)) == -1)
        unix_error("open error");
    
    return fd;
}

void Close(int fd)
{
    int rc;

    if ((rc = close(fd)) == -1)
        unix_error("close error");
    
    // close only returns 0 or -1, because we have handled -1, only 0 is returned
    // There is no need to return from Close
}

ssize_t Write(int fd, const void *buf, size_t n)
{
    ssize_t nwrite;

    if ((nwrite = write(fd, buf, n)) == -1)
        unix_error("write error");
    
    return nwrite;
}

int main(int argc, char *argv[])
{
    struct stat stat;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(0);
    }

    int fd = Open(argv[1], O_RDONLY, 0);
    Fstat(fd, &stat);

    void *bufp = Mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    Write(STDOUT_FILENO, bufp, stat.st_size);

    Munmap(bufp, stat.st_size);
    Close(fd);

    exit(0);
}