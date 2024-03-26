#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    const char *name = "Time_Shared_Memory";
    const int SIZE = sizeof(struct timeval);
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    struct timeval *start = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid_t pid = fork();
    if (pid == 0) { 
        gettimeofday(start, NULL);
        printf("1\n");
        execvp(argv[1], &argv[1]);
        
        exit(1);
    } else if (pid > 0) { 
        wait(NULL); 

        struct timeval end, elapsed;
        gettimeofday(&end, NULL);
        timersub(&end, start, &elapsed);

        printf("Elapsed Time: %ld.%06ld seconds\n", (long)elapsed.tv_sec, (long)elapsed.tv_usec);
        printf("2\n");
        munmap(start, SIZE);
        close(shm_fd);
        shm_unlink(name);
    } else {
        return 1;
    }

    return 0;
}
