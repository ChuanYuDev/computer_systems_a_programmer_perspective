#include "helper.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void handle_client(int connfd);
void client_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
void read_request_headers(rio_t *rp);

int main(int argc, char **argv)
{
    int listenfd, connfd, rc;

    char hostname[MAXLINE], port[MAXLINE];

    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    printf("%s", user_agent_hdr);

    /* Listen for incoming connections */
    if ((listenfd = open_listenfd(argv[1])) < 0)
    {
        /* Open_listenfd can terminate the program because the connection is not established */
        exit(0);
    }

    while(1)
    {
        clientlen = sizeof(clientaddr);

        if ((connfd = accept(listenfd, (SA *)&clientaddr, &clientlen)) < 0)
        {
            continue;
        }

        if ((rc = Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0)) != 0)
        {
            Close(connfd);
            continue;
        }

        printf("Accepted connection from (hostname: %s, port: %s)\n", hostname, port);

        handle_client(connfd);

        Close(connfd);
    }

    /* Establish connection to web server */

    return 0;
}

/* Handle HTTP request */
void handle_client(int connfd)
{
    // Need to read from connfd then write?
    client_error(connfd, "test", "404", "testshort","testlong");

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

    // printf("%s", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));

    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));

    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

/* Read HTTP request headers */
void read_request_headers(rio_t *rp)
{
    char line[MAXLINE];

    do
    {
        Rio_readlineb(rp, line, MAXLINE);
        printf("%s", line);
    } while (strcmp(line, "\r\n"));

}