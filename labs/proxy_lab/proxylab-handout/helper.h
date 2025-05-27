#ifndef __HELPER_H__
#define __HELPER_H__

/*
 * getaddrinfo(), freeaddrinfo(), gai_strerror()
 * Since glibc 2.22:
 * _POSIX_C_SOURCE >= 200112L
 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct sockaddr SA;

/* Internal buffer for robust I/O package */
#define RIO_BUFSIZE 8192
typedef struct
{
    int rio_fd;                 /* Descriptor for this internal buf */
    int rio_cnt;                /* Unread bytes in internal buf */
    char *rio_bufptr;           /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE];  /* Internal buffer */
} rio_t;

/* Misc constants */
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

/* Our own error-handling functions */
// void unix_error(char *msg);
// void gai_error(int code ,char *msg);

/* Unix I/O wrappers */
int Close(int fd);

/* Sockets interface wrappers */
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);

/* Protocol independent wrappers */
int Getaddrinfo(const char *name, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);

/* Rio (Robust I/O) package */
ssize_t Rio_readn(int fd, void *usrptr, size_t n);
// ssize_t rio_writen(int fd, void *usrbuf, size_t n);
ssize_t Rio_writen(int fd, void *usrbuf, size_t n);

void rio_readinitb(rio_t *rp, int fd);
// void Rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* Reentrant protocol-independent client/server helpers */
int open_clientfd(char *hostname, char *port);

int open_listenfd(char *port);
// int Open_listenfd(char *port);

#endif /* __HELPER_H__ */