#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Shared array for Fibonacci numbers
int *fib_sequence;

// Thread function to generate Fibonacci numbers
void *fibonacci_thread(void *param) {
    int n = *(int *)param;
    if (n == 0) {
        fib_sequence[0] = 0;
        pthread_exit(0);
    } else if (n == 1) {
        fib_sequence[0] = 0;
        fib_sequence[1] = 1;
        pthread_exit(0);
    } else {
        fib_sequence[0] = 0;
        fib_sequence[1] = 1;
        for (int i = 2; i <= n; i++) {
            fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];
        }
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <n>\n", argv[0]);
        printf("where <n> is the number of Fibonacci numbers to generate.\n");
        return -1;
    }

    int n = atoi(argv[1]);
    if (n < 0) {
        printf("Please enter a non-negative integer.\n");
        return -1;
    }

    // Allocate memory for the Fibonacci sequence
    fib_sequence = malloc((n + 1) * sizeof(int));
    if (fib_sequence == NULL) {
        perror("Failed to allocate memory");
        return -1;
    }

    // Create the thread
    pthread_t thread;
    pthread_create(&thread, NULL, fibonacci_thread, &n);

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    // Output the Fibonacci sequence
    printf("Fibonacci sequence up to %d numbers:\n", n);
    for (int i = 0; i <= n; i++) {
        printf("%d ", fib_sequence[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(fib_sequence);

    return 0;
}
