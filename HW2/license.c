#include <pthread.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  // Initialize the mutex

// Function to decrease the count of available resources
int decrease_count(int count) {
    pthread_mutex_lock(&mtx);  // Lock the mutex
    while (available_resources < count) {  // If not enough resources, block this thread
        pthread_mutex_unlock(&mtx);  // Unlock mutex to allow other operations
        // Here, we could use a condition variable to wait instead of busy waiting
        pthread_mutex_lock(&mtx);  // Re-lock the mutex when we retry
    }
    available_resources -= count;
    pthread_mutex_unlock(&mtx);  // Unlock the mutex
    return 0;
}

// Function to increase the count of available resources
int increase_count(int count) {
    pthread_mutex_lock(&mtx);  // Lock the mutex
    available_resources += count;
    pthread_mutex_unlock(&mtx);  // Unlock the mutex
    return 0;
}

