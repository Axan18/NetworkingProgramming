// Szkielet klienta TCP/IPv4.
//
// Po podmienieniu SOCK_STREAM na SOCK_DGRAM staje się on szkieletem klienta
// UDP/IPv4 korzystającego z gniazdka działającego w trybie połączeniowym.

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <ctype.h>

bool is_safe_char(unsigned char c) {
    return isprint(c) || c == '\n' || c == '\r' || c == '\t';
}

int main(int argc, char* argv[])
{
    if (argc!=2){
        perror("Pass 1 argument - number of port");
    }
    int port = strtol(argv[2], NULL, 0);
    if (port<1024 || port>65535){
        printf ("port: %d",port);
        perror("Port number wrong");
    }
    const char* server_ip = argv[1];
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    if (inet_pton(AF_INET, server_ip, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    rc = connect(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("connect");
        return 1;
    }

    unsigned char buf[16];
    memcpy(buf, "ping", 4);

    cnt = write(sock, buf, 4);
    if (cnt == -1) {
        perror("write");
        return 1;
    }
    printf("wrote %zi bytes\n", cnt);

    cnt = read(sock, buf, 16);
    if (cnt == -1) {
        perror("read");
        return 1;
    }
    for (ssize_t i = 0; i < cnt; i++) {
        if (is_safe_char(buf[i])) {
            putchar(buf[i]);
        } else {
            printf("\\x%02X", buf[i]); // Jeśli znak jest niebezpieczny, wypisujemy go jako kod szesnastkowy
        }
    }
    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}