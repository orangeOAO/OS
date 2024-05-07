#include <iostream>
#include <pthread.h>

#define MAX_TERMS 1000

int fib[MAX_TERMS]={0};
int n; // Number of Fibonacci terms to generate
int fib_index = 0; // Index to track the next Fibonacci number to print

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *fibonacci(void *arg) {
    int i;
    
    fib[0] = 0;
    fib[1] = 1;

    for (i = 2; i <= n; i++) {
        // Calculate the next Fibonacci number
        fib[i] = fib[i-1] + fib[i-2];

        // Signal the parent thread that a Fibonacci number is ready
        pthread_mutex_lock(&mutex);
        fib_index = i;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        std::cout << "Child thread: Fibonacci number " << fib[i] << " computed.\n";
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int i;

    std::cout << "Enter the number of Fibonacci terms to generate: ";
    std::cin >> n;

    if (n <= 0 || n > MAX_TERMS) {
        std::cout << "Invalid input.\n";
        return 1;
    }

    pthread_create(&tid, NULL, fibonacci, NULL);

    // Print Fibonacci sequence as soon as each number is computed
    std::cout << "Fibonacci sequence:\n";
    for (i = 0; i < n; i++) {
        // Wait for the child thread to compute the next Fibonacci number
        pthread_mutex_lock(&mutex);
        while (fib_index == i) {
            std::cout << "Parent thread waiting...\n";
            std::cout << "fib_index: " << fib_index << ", i: " << i << std::endl;
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        // Print the Fibonacci number
        std::cout << fib[i] << " ";
    }
    std::cout << "\n";

    // Wait for the child thread to finish (optional)
    pthread_join(tid, NULL);

    return 0;
}
