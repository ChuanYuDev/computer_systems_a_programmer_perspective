#include "helper.h"

/* Our own error-handling functions */
/* Unix-style error */
void unix_error(char *msg) 
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
}

void gai_error(int code ,char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, gai_strerror(code));
}

void posix_error(int code, char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
}

void app_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

/* Process control wrappers */

int Kill(pid_t pid, int signum) 
{
    int rc;

    if ((rc = kill(pid, signum)) < 0)
        unix_error("Kill error");
    
    return rc;
}

/* Signal wrappers */
/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");

    return (old_action.sa_handler);
}

/* Unix I/O wrappers */
int Close(int fd)
{
    int rc;
    char buf[MAXBUF];

    if ((rc = close(fd)) < 0)
    {
        sprintf(buf, "Close error, fd: %d", fd);
        unix_error(buf);
    }

    return rc;
}

/* Dynamic storage allocation wrappers */
void *Calloc(size_t nmemb, size_t size)
{
    void *p;

    if ((p = calloc(nmemb, size)) == NULL)
        unix_error("Calloc error");
    
    return p;
}

/* Sockets interface wrappers */
int Listen(int fd, int backlog)
{
    int rc;

    if ((rc = listen(fd, backlog)) < 0)
        unix_error("LIsten error");
    
    return rc;
}

int Accept(int fd, struct sockaddr *addr, socklen_t *addrlen) 
{
    int rc;

    if ((rc = accept(fd, addr, addrlen)) < 0)
        unix_error("Accept error");

    return rc;
}

/* Protocol independent wrappers */
int Getaddrinfo(const char *name, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    int rc;
    char buf[MAXBUF];

    if ((rc = getaddrinfo(name, service, hints, res)) != 0) 
    {
        sprintf(buf, "Getaddrinfo failed (name: %s, service: %s)", name, service);
        gai_error(rc, "Getaddrinfo error");
    }

    return rc;
}

int Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags)
{
    int rc;

    if ((rc = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags)) != 0) 
        gai_error(rc, "Getnameinfo error");
    
    return rc;
}

/* Pthreads thread control wrappers */
int Pthread_create(pthread_t *tidp, void * (*routine)(void *), void *argp)
{
    int rc;

    if ((rc = pthread_create(tidp, NULL, routine, argp)) != 0)
        posix_error(rc, "Pthread_create error");
    
    return rc;
}

int Pthread_detach(pthread_t tid)
{
    int rc;

    if ((rc = pthread_detach(tid)) != 0)
        posix_error(rc, "Pthread_detach error");
    
    return rc;
}

/* POSIX semaphore wrappers */
int Sem_init(sem_t *sem, unsigned int value)
{
    int rc;

    if ((rc = sem_init(sem, 0, value)) < 0)
        unix_error("Sem_init error");
    
    return rc;
}

int P(sem_t *sem)
{
    int rc;

    if ((rc = sem_wait(sem)) < 0)
        unix_error("P error");
    
    return rc;
}

int V(sem_t *sem)
{
    int rc;

    if ((rc = sem_post(sem)) < 0)
        unix_error("Q error");
    
    return rc;
}

/* Rio (Robust I/O) package */
/*
 * rio_readn - Robustly read n bytes (unbuffered)
 */
static ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR) /* Interrupted by sig handler return */
                nread = 0;      /* and call read() again */

            else
                return -1;      /* errno set by read() */ 
        } 

        else if (nread == 0)
            break;              /* EOF */

        nleft -= nread;
        bufp += nread;
    }

    return (n - nleft);         /* Return >= 0 */
}

ssize_t Rio_readn(int fd, void *usrptr, size_t n) 
{
    ssize_t rc;
  
    if ((rc = rio_readn(fd, usrptr, n)) < 0)
        unix_error("Rio_readn error");

    return rc;
}

/*
 * rio_writen - Robustly write n bytes (unbuffered)
 */

static ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, bufp, nleft)) <= 0)
        {
            if (errno == EINTR)  /* Interrupted by sig handler return */
                nwritten = 0;    /* and call write() again */

            else
                return -1;       /* errno set by write() */
        }

        nleft -= nwritten;
        bufp += nwritten;
    }

    return n;
}

ssize_t Rio_writen(int fd, void *usrbuf, size_t n)
{
    ssize_t rc;

    if ((rc = rio_writen(fd, usrbuf, n)) != n)
        unix_error("Rio_writen error");
    
    return rc;
}

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
void rio_readinitb(rio_t *rp, int fd) 
{
    rp->rio_fd = fd;  
    rp->rio_cnt = 0;  
    rp->rio_bufptr = rp->rio_buf;
}

/* 
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 *  
 * Return:
 *  -1: errno set by read() function
 *   0: EOF
 *  positive number: the number of bytes transfered from internal buf to usrbuf
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    /* Refill if buf is empty */
    while (rp->rio_cnt <= 0)
    {  
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

        if (rp->rio_cnt < 0)
        {
            if (errno != EINTR) /* Interrupted by sig handler return */
                return -1;
        }

        else if (rp->rio_cnt == 0)  /* EOF */
            return 0;

        else 
            rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;          

    if (rp->rio_cnt < n)   
        cnt = rp->rio_cnt;

    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;

    return cnt;
}

/* 
 * rio_readlineb - Robustly read a text line (buffered)
 *
 * Return:
 *  -1: errno set by rio_read() function
 *   0: EOF
 *  positive number: the number of bytes read (include '\n', exclude '\0')
 */
static ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++)
    { 
        if ((rc = rio_read(rp, &c, 1)) == 1)
        {
            *bufp++ = c;
            if (c == '\n')
            {
                n++;
                break;
            }
        }

        else if (rc == 0)
        {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break;    /* EOF, some data was read */
        }
        
        else
            return -1;	  /* Error */
    }

    *bufp = 0;

    return n - 1;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
        unix_error("Rio_readlineb error");

    return rc;
} 

/* Request line helper function */
void print_rl(request_line_t *rlp)
{
    printf("Request line host: %s, hostname: %s, port: %s, path: %s\n", rlp->host, rlp->hostname, rlp->port, rlp->path);
}

void copy_rl(request_line_t *dest, request_line_t *src)
{
    memcpy(dest, src, sizeof(request_line_t));
}

int is_rl_equal(request_line_t *rl1, request_line_t *rl2)
{
    !strcmp(rl1->hostname, rl2->hostname) && !strcmp(rl1->port, rl2->port) && !strcmp(rl1->path, rl2->path);
}

/* Reentrant protocol-independent client/server helpers */
/*
 * open_clientfd - Open connection to server at <hostname, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
int open_clientfd(char *hostname, char *port)
{
    int clientfd;
    char buf[MAXBUF];
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV;  /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections */

    if ((Getaddrinfo(hostname, port, &hints, &listp)) != 0)
    {
        return -2;
    }
  
    /* Walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next)
    {
        /* Create a socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue; /* Socket failed, try the next */

        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) 
            break; /* Success */

        /* Connect failed, try another */  //line:netp:openclientfd:closefd
        if (Close(clientfd) < 0)
        { 
            return -1;
        } 
    } 

    /* Clean up */
    freeaddrinfo(listp);

    /* All connects failed */
    if (!p)
    {
        sprintf(buf, "open_clientfd error, all connects failed to server: %s:%s", hostname, port);
        app_error(buf);
        return -1;
    }
    else    /* The last connect succeeded */
        return clientfd;
}

/*  
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
int open_listenfd(char *port) 
{
    struct addrinfo hints, *listp, *p;
    char buf[MAXBUF];
    int listenfd, rc, optval=1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */

    if ((rc = Getaddrinfo(NULL, port, &hints, &listp)) != 0)
    {
        return -2;
    }

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next)
    {
        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue;  /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,    //line:netp:csapp:setsockopt
                   (const void *)&optval , sizeof(int));

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */

        /* Bind failed, try the next */
        if (Close(listenfd) < 0)
        {
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);

    /* No address worked */
    if (!p) 
    {
        sprintf(buf, "open_listenfd error, no address worked, port: %s", port);
        app_error(buf);
        return -1;
    }

    /* Make it a listening socket ready to accept connection requests */
    if (Listen(listenfd, LISTENQ) < 0)
    {
        Close(listenfd);
        return -1;
    }

    return listenfd;
}