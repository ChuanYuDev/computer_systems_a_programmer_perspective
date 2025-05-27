#include "helper.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void handle_client(int connfd);
void client_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
void parse_uri(char *uri, char *host, char *hostname, char *port, char *path);

void handle_server(int clientfd, int connfd, char *server_host, char *server_path, rio_t *client_rp);
void read_headers(rio_t *rp);

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
            printf("Accepted client connection from %s:%s\n", client_hostname, client_port);
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
    char server_host[MAXLINE], server_hostname[MAXLINE], server_port[MAXLINE], server_path[MAXLINE];
    int clientfd;
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

    parse_uri(uri, server_host, server_hostname, server_port, server_path);
    printf("Server host: %s, hostname: %s, port: %s, path: %s\n", server_host, server_hostname, server_port, server_path);

    /* If server hostname is "\0", send error to client, 400 Bad request */
    if (server_hostname[0] == '\0')
    {
        client_error(connfd, server_hostname, "400", "Bad request", "Proxy can't find server hostname");
        return;
    }

    /* Establish connection to web server */
    if ((clientfd = open_clientfd(server_hostname, server_port)) < 0)
    {
        client_error(connfd, server_host, "400", "Bad request", "Proxy can't connect to server");
        return;
    }

    printf("Connected to server %s:%s\n", server_hostname, server_port);

    handle_server(clientfd, connfd, server_host, server_path, &client_rio);

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

/*
 * Parse uri
 * Separate hostname, port and path
 * If no port is provided, use the default port 80
 * 
 * host: hostname or hostname:port based on uri
 */
void parse_uri(char *uri, char *host, char *hostname, char *port, char *path)
{
    char uri_copy[MAXLINE];
    char *hostname_ptr, *port_ptr, *path_ptr;
    char *double_slash = "://", *default_port = "80";

    strcpy(uri_copy, uri);
    // printf("%s\n", uri_copy);

    hostname_ptr = strstr(uri_copy, double_slash);

    if(!hostname_ptr)
        hostname_ptr = uri_copy;
    
    else
        hostname_ptr += strlen(double_slash);
    
    path_ptr = strstr(hostname_ptr, "/");
    strcpy(path, path_ptr);
    *path_ptr = '\0';

    strcpy(host, hostname_ptr);

    port_ptr = strstr(hostname_ptr, ":");

    if (!port_ptr)
        strcpy(port, default_port);
    
    else
    {
        strcpy(port, port_ptr + 1);
        *port_ptr = '\0';
    }

    strcpy(hostname, hostname_ptr);
}

/* Handle server */
void handle_server(int clientfd, int connfd, char *server_host, char *server_path, rio_t *client_rp)
{

    char buf[MAXBUF], line[MAXLINE];
    rio_t server_rio;

    rio_readinitb(&server_rio, clientfd);

    /* Send HTTP request line with HTTP/1.0 to server */
    sprintf(buf, "GET %s HTTP/1.0\r\n", server_path);
    Rio_writen(clientfd, buf, strlen(buf));

    /* Send HTTP request to server */
    printf("Client request headers:\n");

    // TO DO: Modify HTTP request, first just send client header to the server without change
    do
    {
        Rio_readlineb(client_rp, line, MAXLINE);
        printf("%s", line);

        Rio_writen(clientfd, line, strlen(line));
    }
    while (strcmp(line, "\r\n"));

    /* Receive server HTTP response */
    printf("Server request headers:\n");
    do
    {
        Rio_readlineb(&server_rio, line, MAXLINE);
        printf("%s", line);

        /* Send HTTP response header to the client directly */
        Rio_writen(connfd, line, strlen(line));
    }
    while (strcmp(line, "\r\n"));


    /* Read content-length and allocate enough memory to hold the response body */

    /* Send HTTP response body to the client via Rio_writen */
}


/* Read HTTP request headers */
void read_headers(rio_t *rp)
{
    char line[MAXLINE];

    do
    {
        Rio_readlineb(rp, line, MAXLINE);
        printf("%s", line);
    } while (strcmp(line, "\r\n"));

}
