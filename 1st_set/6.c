#include<fcntl.h>
#include<stdbool.h>
#include<stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
    if(argc<3){
        perror("You should pass 2 file paths");
        return 1;
    }
    int sourceDesc = open(argv[1], O_RDONLY);
    if (sourceDesc<0){
        perror("File opening error");
        return 1;
    }
    int destDesc = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC ,0644);
    if (destDesc<0){
        perror("File opening error");
        close(sourceDesc);
        return 1;
    }
    char buffer[10];
    ssize_t n_bytes = 0;
    while(true){
        n_bytes = read(sourceDesc, buffer, 10);
        if(n_bytes<0){
            perror("Reading error");
            close(sourceDesc);
            close(destDesc);
            return -1;
        }
        ssize_t written = 0;
        while (written < n_bytes) {
            ssize_t w = write(destDesc, buffer + written, n_bytes - written);
            if (w < 0) {
                perror("Writing error");
                close(sourceDesc);
                close(destDesc);
                return 1;
            }
            printf("Written: %ld \n",w);
            written += w;
        }
        if(n_bytes<10) break;
    }
    close(sourceDesc);
    close(destDesc);
    return 0;
}