#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024


int main(int argc, char *argv[]){
    char srcFileName[256], destFileName[256];
    int srcFd, destFd, readBytes;
    char buffer[BUFFER_SIZE];


    if(argc<3)
    {
        printf("Usage: %s <srcFile> <desFile>\n", argv[0]);
        return 1;
    }
    int fd[2];
    if (pipe(fd) == -1) 
    {
        return 1;
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        return 0;
    }
    else if (pid == 0) 
    {
        close(fd[0]);
        srcFd = open(argv[1], O_RDONLY);
        if (srcFd == -1) 
        {
            perror("Error opening source file");
            return 1;
        }

        
        while ((readBytes = read(srcFd, buffer, BUFFER_SIZE)) > 0)
        {
            write(fd[1], buffer, readBytes);
            
        }
        close(fd[1]);
        close(srcFd);

    }
    else if(pid > 0)
    {
        close(fd[1]); // 父进程不需要写入管道，关闭写端
        destFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (destFd == -1) 
        {
            perror("Error creating destination file");
            close(srcFd);
            return 1;
        }
        int destFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (destFd == -1) {
            perror("Error creating destination file");
            return 1;
        }

        char buffer[BUFFER_SIZE];
        ssize_t readBytes;
        while ((readBytes = read(fd[0], buffer, BUFFER_SIZE)) > 0) {
            write(destFd, buffer, readBytes);
        }
        close(destFd);
        close(fd[0]); // 读完数据后关闭读端

        wait(NULL); // 等待子进程结束
    }
    return 0;
}