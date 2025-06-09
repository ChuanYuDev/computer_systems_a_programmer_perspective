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

#include <signal.h>

#include <pthread.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct sockaddr SA;

/* Misc constants */
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

/* Internal buffer for robust I/O package */
#define RIO_BUFSIZE 8192
typedef struct
{
    int rio_fd;                 /* Descriptor for this internal buf */
    int rio_cnt;                /* Unread bytes in internal buf */
    char *rio_bufptr;           /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE];  /* Internal buffer */
} rio_t;

/* Request line structure */
#define RL_BUFSIZE 1024
typedef struct
{
    char host[RL_BUFSIZE];      /* host: hostname or hostname:port based on uri */
    char hostname[RL_BUFSIZE];
    char port[RL_BUFSIZE];
    char path[RL_BUFSIZE];
}request_line_t;

/* Our own error-handling functions */
void unix_error(char *msg);
void gai_error(int code ,char *msg);
void posix_error(int code, char *msg);

void app_error(char *msg);

/* Process control wrappers */
int Kill(pid_t pid, int signum);

/* Signal wrappers */
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/* Unix I/O wrappers */
int Close(int fd);

/* Dynamic storage allocation wrappers */
void *Calloc(size_t nmemb, size_t size);

/* Sockets interface wrappers */
int Listen(int fd, int backlog);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);

/* Protocol independent wrappers */
int Getaddrinfo(const char *name, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);

/* Pthreads thread control wrappers */
int Pthread_create(pthread_t *tidp, void * (*routine)(void *), void *argp);
int Pthread_detach(pthread_t tid);

/* POSIX semaphore wrappers */
int Sem_init(sem_t *sem, unsigned int value);
int P(sem_t *sem);
int V(sem_t *sem);

/* Rio (Robust I/O) package */
ssize_t Rio_readn(int fd, void *usrptr, size_t n);
ssize_t Rio_writen(int fd, void *usrbuf, size_t n);

void rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* Request line helper function */
void rl_print(request_line_t *rlp);
void rl_copy(request_line_t *dest, request_line_t *src);
int is_rl_equal(request_line_t *rl1, request_line_t *rl2);

/* Reentrant protocol-independent client/server helpers */
int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);

#endif /* __HELPER_H__ */