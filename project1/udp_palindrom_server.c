#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

bool is_palindrom(const unsigned char* wordStart, int wordLength){
    const unsigned char* left = wordStart;
    const unsigned char* right = left + wordLength - 1;
    while (left < right) {
        if (*left != *right)
            return false;
        left++;
        right--;
    }
    return true;
}

bool word_parser(unsigned char* wordStart, int wordLength){
    unsigned char* p = wordStart;
    for (int i = 0; i < wordLength; i++) {
        if (*p < 'A' || (*p > 'Z' && *p < 'a') || *p > 'z')
            return false;
        if (*p >= 'A' && *p <= 'Z')
            *p += ('a' - 'A');
        p++;
    }
    return true;
}

char* datagram_stream(unsigned char* buff){
    unsigned char* letter = buff;
    if (*letter == ' ') return NULL;
    
    unsigned char* wordStart = letter;
    int wordCount = 0, palindromCount = 0, wordLength = 0;

    while (*letter) {
        if (*letter == ' ') { // end of the word
            if (*(letter + 1) == ' ' || *(letter + 1) == '\0') // if two spaces together || space on the end -> ERROR
                return NULL;
            if (wordLength > 0 && word_parser(wordStart, wordLength)) { //word processing
                wordCount++;
                if (is_palindrom(wordStart, wordLength))
                    palindromCount++;
            }else{
                return NULL;
            }
            wordStart = letter + 1;
            wordLength = 0;
        } else {
            wordLength++;
        }
        letter++;
    }

    if (wordLength > 0 && word_parser((unsigned char*)wordStart, wordLength)) { // last word
        wordCount++;
        if (is_palindrom(wordStart, wordLength))
            palindromCount++;
    }else{
        return NULL;
    }

    char* result = malloc(22);
    if (!result) return NULL;

    snprintf(result, 22, "%d/%d", palindromCount, wordCount);
    return result;
}

bool containsZeroByte(unsigned char* buf, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (buf[i] == '\0') {
            return true;
        }
    }
    return false;
}

int main(void) {
    int sock, rc;
    ssize_t cnt;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(2020)
    };

    rc = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        return 1;
    }

    while (1) {
        unsigned char buf[1025]={0};
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len = sizeof(clnt_addr);

        cnt = recvfrom(sock, buf, sizeof(buf), 0, 
                       (struct sockaddr*)&clnt_addr, &clnt_addr_len);
        if (cnt == -1) {
            perror("recvfrom");
            return -1;
        }
        unsigned char send_buf[8]={0};
        if(cnt==0){
            strcpy((char*)send_buf, "0/0");            
        }else if (cnt > 1024 || containsZeroByte(buf,cnt)){
            strcpy((char*)send_buf, "ERROR");
        }
        else{
            buf[cnt] = '\0';
            char* result = datagram_stream(buf);
            if (!result) {
                strcpy((char*)send_buf, "ERROR");
                send_buf[sizeof(send_buf) - 1] = 0;
            } else {
                strcpy((char*)send_buf, result);
                send_buf[sizeof(send_buf) - 1] = 0;
                free(result);
            }
        }
        cnt = sendto(sock, send_buf, strlen((char*)send_buf), 0,
                    (struct sockaddr*)&clnt_addr, clnt_addr_len);

        if (cnt == -1) {
            perror("sendto");
            return 1;
        }
    }

    close(sock);
    return 0;
}
