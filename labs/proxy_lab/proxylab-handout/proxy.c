#include <stdio.h>
#include <stdlib.h>

#include <helper.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int main(int argc, char **argv)
{
    char *port;

    // if (argc != 2)
    // {
    //     fprintf(stderr, "usage: %s <port>\n", argv[0]);
    //     exit(0);
    // }

    // port = argv[2];
    port = "2702";

    printf("%s", user_agent_hdr);

    /* Establish connection to web server */

    return 0;
}

/* Send error message to client */
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
    char buf[MAXBUF], body[MAXLINE];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Proxy Error</title>");
    sprintf(body, "%s<body bgcolor=ffffff>\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The proxy</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));

    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));

    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}