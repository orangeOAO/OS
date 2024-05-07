#include <iostream>
#include <pthread.h>
#include <unistd.h>

const int NUM_PHILOSOPHERS = 5;

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];
pthread_cond_t conditions[NUM_PHILOSOPHERS];
pthread_mutex_t print_count;

enum State { THINKING, HUNGRY, EATING };

State states[NUM_PHILOSOPHERS];

void test(int i) 
{
    int left = i;
    int right = (i + 1) % NUM_PHILOSOPHERS;
    if (states[i] == HUNGRY &&
        states[left] != EATING &&
        states[right] != EATING) 
    {
        states[i] = EATING;
        pthread_cond_signal(&conditions[i]);
    }
}

void pickup(int i) {
    pthread_mutex_lock(&chopsticks[i]);
    states[i] = HUNGRY;
    test(i);
    while (states[i] != EATING) {
        pthread_cond_wait(&conditions[i], &chopsticks[i]);
    }
    pthread_mutex_unlock(&chopsticks[i]);
}

void putdown(int i) {
    pthread_mutex_lock(&chopsticks[i]);
    states[i] = THINKING;
    test((i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
    test((i + 1) % NUM_PHILOSOPHERS);
    pthread_mutex_unlock(&chopsticks[i]);
}

void* philosopher(void* num) {
    int i = *(int*) num;

    while (true) {
        pthread_mutex_lock(&print_count);
        std::cout << "Philosopher " << i << " is thinking.\n";
        pthread_mutex_unlock(&print_count);
        sleep(1);
        pickup(i);
        pthread_mutex_lock(&print_count);
        std::cout << "Philosopher " << i << " is eating.\n";
        pthread_mutex_unlock(&print_count);
        sleep(1);
        putdown(i);
    }
    return nullptr;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int thread_ids[NUM_PHILOSOPHERS];

    pthread_mutex_init(&print_count,nullptr);

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&chopsticks[i], nullptr);
        pthread_cond_init(&conditions[i], nullptr);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], nullptr, philosopher, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&chopsticks[i]);
        pthread_cond_destroy(&conditions[i]);
    }

    return 0;
}
