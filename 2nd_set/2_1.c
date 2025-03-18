#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
int main(int argc, char* args[])
{
    if (argc!=2){
        perror("Pass 1 argument - number of port");
    }
    int port = strtol(args[1], NULL, 0);
    if (port<1024 || port>65535){
        printf ("port: %d",port);
        perror("Port number wrong");
    }
    int lst_sock;
    int clnt_sock;
    int rc;
    ssize_t cnt;

    lst_sock = socket (AF_INET, SOCK_STREAM, 0);
    if (lst_sock == -1) {
        perror ("socket");
        return 1;
    }
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons (port)
    };
    rc = bind(lst_sock, (struct sockaddr *) & addr, sizeof (addr));
    if (rc == -1) {
        perror ("bind");
        return 1;
    }
    rc = listen(lst_sock, 10);
    if (rc == -1) {
        perror ("listen") ;
        return 1;
    }
    bool keep_on_handling_clients = true;
    while (keep_on_handling_clients) {
        clnt_sock = accept (lst_sock, NULL, NULL);
        if (clnt_sock == -1) {
            perror ("accept") ;
            return 1;
        }
        unsigned char buff[16] = "Hello, World! \r\n";
        cnt = write(clnt_sock, buff, 16);
        if (cnt == -1) {
            perror ("write");
            return 1;
        }
        rc = close(clnt_sock);
        if (rc == -1) {
            perror ("close");
            return 1;
        }
    }
    rc = close (lst_sock);
    if (rc == -1) {
        perror ("close");
        return 1;
    }
    return 0;
}