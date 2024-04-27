#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int process_id;
    int burst_time;
    int priority;
} Process;

int time_quantum = 4;

int compare_priority(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->priority - p2->priority;
}

void priorityRoundRobin(Process processes[], const int n) {
    // Sort processes by priority
    qsort(processes, n, sizeof(Process), compare_priority);

    int total_time = 0, done = 0, current = 0;
    int remain_burst[n];
    for (int i = 0; i < n; i++) {
        remain_burst[i] = processes[i].burst_time;
    }

    printf("Process ID\tPriority\tBurst Time\tTime Quantum\n");

    while (!done) {
        done = 1; // Assume all processes are handled until proven otherwise
        for (int i = 0; i < n; i++) {
            if (remain_burst[i] > 0) {
                done = 0; // There is still a process that needs to be handled
                if (remain_burst[i] > time_quantum) {
                    total_time += time_quantum;
                    remain_burst[i] -= time_quantum;
                    printf("Process[%d]\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].priority, remain_burst[i], time_quantum);
                } else {
                    total_time += remain_burst[i];
                    printf("Process[%d]\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].priority, 0, remain_burst[i]);
                    remain_burst[i] = 0;
                }
            }
        }
    }

    printf("\nTotal Time Spent: %d\n", total_time);
}

int main() {
    Process processes[] = {{1, 10, 1}, {2, 15, 2}, {3, 20, 1}, {4, 20, 2}};
    int n = sizeof(processes)/sizeof(processes[0]);
    priorityRoundRobin(processes, n);
    return 0;
}
