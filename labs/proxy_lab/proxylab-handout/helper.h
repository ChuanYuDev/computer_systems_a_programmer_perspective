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
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* Misc constants */
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

/* Our own error-handling functions */
void unix_error(char *msg);

/* Rio (Robust I/O) package */
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void Rio_writen(int fd, void *usrbuf, size_t n);

/* Reentrant protocol-independent client/server helpers */
int open_clientfd(char *hostname, char *port);

int open_listenfd(char *port);
int Open_listenfd(char *port);

#endif /* __HELPER_H__ */