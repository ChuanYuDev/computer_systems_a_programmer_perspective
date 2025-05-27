#include "helper.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void handle_client(int connfd);
void client_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
void read_request_headers(rio_t *rp);
void parse_uri(char *uri, char *hostname, char *port, char *path);

int main(int argc, char **argv)
{
    int listenfd, connfd;

    char client_hostname[MAXLINE], client_port[MAXLINE];

    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    printf("%s", user_agent_hdr);

    // char uri[] = "http://www.cmu.edu/hub/index.html";
    // char uri[] = "www.cmu.edu/hub/index.html";
    char uri[] = "www.cmu.edu:8080/hub/index.html";

    char hostname[MAXLINE], port[MAXLINE], path[MAXLINE];
    parse_uri(uri, hostname, port, path);
    exit(0);

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

        if (!Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0))
        {
            printf("Accepted connection from (client hostname: %s, client port: %s)\n", client_hostname, client_port);
        }

        handle_client(connfd);

        Close(connfd);
    }

    return 0;
}

/* Handle HTTP request */
void handle_client(int connfd)
{
    char line[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio;

    rio_readinitb(&rio, connfd);

    if (Rio_readlineb(&rio, line, MAXLINE) < 0)
    {
        return;
    }

    printf("Request line: %s", line);
    sscanf(line, "%s %s %s", method, uri, version);

    if (strcasecmp(method, "GET"))
    {
        client_error(connfd, method, "501", "Not implemented", "Proxy does not implement this method");
        return;
    }

    // Need to read from connfd then write?
    client_error(connfd, "test", "404", "testshort","testlong");

    // Test read
    read_request_headers(&rio);

    /* Parse uri */
    /* Establish connection to web server */

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

void parse_uri(char *uri, char *hostname, char *port, char *path)
{
    char *hostname_ptr, *port_ptr, *path_ptr;
    char *double_slash = "://", *default_port = "80";

    printf("%s\n", uri);

    hostname_ptr = strstr(uri, double_slash);

    if(!hostname_ptr)
        hostname_ptr = uri;
    
    else
        hostname_ptr += strlen(double_slash);
    
    path_ptr = strstr(hostname_ptr, "/");
    strcpy(path, path_ptr);
    *path_ptr = '\0';

    port_ptr = strstr(hostname_ptr, ":");

    if (!port_ptr)
        strcpy(port, default_port);
    
    else
    {
        strcpy(port, port_ptr + 1);
        *port_ptr = '\0';
    }

    strcpy(hostname, hostname_ptr);

    printf("%s, %s, %s\n", hostname, port, path);
}