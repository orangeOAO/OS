#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        struct timeval start;
        gettimeofday(&start, NULL);
        write(fd[1], &start, sizeof(start));
        close(fd[1]);

        execvp(argv[1], &argv[1]);
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
        close(fd[1]);

        struct timeval start, end, elapsed;
        read(fd[0], &start, sizeof(start));
        close(fd[0]);

        gettimeofday(&end, NULL);
        timersub(&end, &start, &elapsed);

        printf("Elapsed Time: %ld.%06ld seconds\n", (long)elapsed.tv_sec, (long)elapsed.tv_usec);
    } else {
        return 1;
    }

    return 0;
}
