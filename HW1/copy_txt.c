#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main() {
    char srcFileName[256], destFileName[256];
    int srcFd, destFd, readBytes;
    char buffer[BUFFER_SIZE];

    printf("Enter source file name: ");
    scanf("%255s", srcFileName);
    printf("Enter destination file name: ");
    scanf("%255s", destFileName);

    srcFd = open(srcFileName, O_RDONLY);
    if (srcFd == -1) {
        perror("Error opening source file");
        return 1;
    }

    destFd = open(destFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) {
        perror("Error creating destination file");
        close(srcFd);
        return 1;
    }

    while ((readBytes = read(srcFd, buffer, BUFFER_SIZE)) > 0) {
        if (write(destFd, buffer, readBytes) != readBytes) {
            perror("Error writing to destination file");
            close(srcFd);
            close(destFd);
            return 1;
        }
    }

    if (readBytes == -1) {
        perror("Error reading from source file");
    }

    close(srcFd);
    close(destFd);

    return 0;
}
