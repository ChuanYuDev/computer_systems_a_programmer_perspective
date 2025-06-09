#include "helper.h"
#include "sbuf.h"

#define N_THERADS 4
#define SBUF_SIZE 16

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
/* This User-Agent header will lead to 302 FOUND error when access www.google.com, while the below curl User-Agent will return 200 OK */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
// static const char *user_agent_hdr = "User-Agent: curl/7.81.0\r\n";

void *thread(void *vargp);
void handle_client(int connfd);
void client_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
void parse_uri(char *uri, rl_t *rlp);
void handle_server(int clientfd, int connfd, rl_t *client_rlp, rio_t *client_rp);
void sigint_handler(int sig);

sbuf_t sbuf;

int main(int argc, char **argv)
{
    int listenfd, connfd;

    char client_hostname[MAXLINE], client_port[MAXLINE];

    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    pthread_t tid;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    Signal(SIGINT,  sigint_handler);   /* ctrl-c */

    /* Listen for incoming connections */
    if ((listenfd = open_listenfd(argv[1])) < 0)
    {
        /* Open_listenfd can terminate the program because the connection is not established */
        exit(0);
    }

    sbuf_init(&sbuf, SBUF_SIZE);

    for (int i = 0; i < N_THERADS; ++i)
        Pthread_create(&tid, thread, NULL);

    while(1)
    {
        clientlen = sizeof(clientaddr);

        if ((connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen)) < 0)
        {
            continue;
        }

        if (!Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0))
        {
            printf("\nAccepted client connection from %s:%s\n", client_hostname, client_port);
        }

        sbuf_insert(&sbuf, connfd);
    }

    return 0;
}

void *thread(void *vargp)
{
    int connfd;

    Pthread_detach(pthread_self());

    while(1)
    {
        connfd = sbuf_remove(&sbuf); 
        handle_client(connfd);
        Close(connfd);
    }
}

/* Handle HTTP request */
void handle_client(int connfd)
{
    char line[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    int clientfd;
    rl_t client_rl;
    rio_t client_rio;

    rio_readinitb(&client_rio, connfd);

    if (Rio_readlineb(&client_rio, line, MAXLINE) < 0)
    {
        return;
    }

    /* Read request line, if the mehod is not `GET`, send error to client, 501 Not implemented */
    printf("Client request line:\n%s", line);
    sscanf(line, "%s %s %s", method, uri, version);

    if (strcasecmp(method, "GET"))
    {
        client_error(connfd, method, "501", "Not implemented", "Proxy does not implement this method");
        return;
    }

    parse_uri(uri, &client_rl);
    print(&client_rl);

    /* If server hostname is "\0", send error to client, 400 Bad request */
    if (client_rl.hostname[0] == '\0')
    {
        client_error(connfd, client_rl.hostname, "400", "Bad request", "Proxy can't find server hostname");
        return;
    }

    /* Establish connection to web server */
    if ((clientfd = open_clientfd(client_rl.hostname, client_rl.port)) < 0)
    {
        /* If open_clientfd failed, send message back to client */
        client_error(connfd, client_rl.host, "400", "Bad request", "Proxy can't connect to server");
        return;
    }

    printf("Connected to server %s:%s\n", client_rl.hostname, client_rl.port);

    handle_server(clientfd, connfd, &client_rl, &client_rio);

    Close(clientfd);

}

/* Send error message to client */
void client_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
    char buf[MAXBUF], body[MAXLINE];

    /* Build the HTTP response body */
    sprintf(body,
        "<html><title>Proxy Error</title><body bgcolor=ffffff>\r\n"
        "%s: %s\r\n"
        "<p>%s: %s\r\n"
        "<hr><em>The proxy</em>\r\n", errnum, shortmsg, longmsg, cause
    );

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));

    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));

    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

/*
 * Parse uri
 * Separate hostname, port and path
 * If no port is provided, use the default port 80
 */
void parse_uri(char *uri, rl_t *rlp)
{
    char uri_copy[MAXLINE];
    char *hostname_ptr, *port_ptr, *path_ptr;
    char *double_slash = "://", *default_port = "80";

    strcpy(uri_copy, uri);

    hostname_ptr = strstr(uri_copy, double_slash);

    if(!hostname_ptr)
        hostname_ptr = uri_copy;
    
    else
        hostname_ptr += strlen(double_slash);
    
    path_ptr = strstr(hostname_ptr, "/");
    strcpy(rlp->path, path_ptr);
    *path_ptr = '\0';

    strcpy(rlp->host, hostname_ptr);

    port_ptr = strstr(hostname_ptr, ":");

    if (!port_ptr)
        strcpy(rlp->port, default_port);
    
    else
    {
        strcpy(rlp->port, port_ptr + 1);
        *port_ptr = '\0';
    }

    strcpy(rlp->hostname, hostname_ptr);
}

/* Handle server */
void handle_server(int clientfd, int connfd, rl_t *client_rlp, rio_t *client_rp)
{
    char buf[MAXBUF], line[MAXLINE];
    ssize_t read_bytes;
    char contain_host = 0;

    char *user_agent_name = "User-Agent", *connection_name = "Connection", *proxy_connection_name = "Proxy-Connection";

    /* Send HTTP request line with HTTP/1.0 to server */
    sprintf(buf, "GET %s HTTP/1.0\r\n", client_rlp->path);
    Rio_writen(clientfd, buf, strlen(buf));

    /* Send HTTP request to server */
    printf("Client request headers:\n");

    Rio_readlineb(client_rp, line, MAXLINE);

    while (strcmp(line, "\r\n"))
    {
        printf("%s", line);

        if (!contain_host && strstr(line, "Host:"))
            contain_host = 1;

        /* Do not send client User-Agent, Connection, Proxy-Connection headers to server */
        if (!strstr(line, user_agent_name) && !strstr(line, connection_name) && !strstr(line, proxy_connection_name))
            Rio_writen(clientfd, line, strlen(line));

        Rio_readlineb(client_rp, line, MAXLINE);
    }

    if (!contain_host)
    {
        sprintf(buf, "Host: %s\r\n", client_rlp->host);
        Rio_writen(clientfd, buf, strlen(buf));
    }

    /* Send User-Agent to server */
    strcpy(buf, user_agent_hdr);
    Rio_writen(clientfd, buf, strlen(buf));

    /* Send Connection to server */
    sprintf(buf, "%s: close\r\n", connection_name);
    Rio_writen(clientfd, buf, strlen(buf));

    /* Send Proxy-Connection and \r\n to server */
    sprintf(buf, "%s: close\r\n\r\n", proxy_connection_name);
    Rio_writen(clientfd, buf, strlen(buf));

    /* Receive server HTTP response and send to the client*/
    do
    {
        read_bytes = Rio_readn(clientfd, buf, MAXBUF);
        Rio_writen(connfd, buf, read_bytes);

        printf("Read bytes from server: %ld\n", read_bytes);
        fflush(stdout);
    }
    while (read_bytes == MAXBUF);
}

void sigint_handler(int sig) 
{
    int olderrno = errno;

    sbuf_deinit(&sbuf);

    printf("Kill proxy process group, process group id: %d\n", getpgrp());
    fflush(stdout);

    // Kill(-getpgrp(), SIGKILL);
    Kill(getpid(), SIGKILL);

    errno = olderrno;
    return;
}