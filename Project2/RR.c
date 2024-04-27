#include <stdio.h>
#include <stdlib.h>
#define TIME_QUANTUM 2

typedef struct {
    int process_id;
    int burst_time;
} Process;

void RoundRobin(Process processes[], int n) {
    int i, total = 0, x, counter = 0, time_quantum;
    int wait_time = 0, turnaround_time = 0, remain;
    int *remaining_burst = malloc(sizeof(int)*n);
    remain = n;
    for (i = 0; i < n; i++)
        remaining_burst[i] = processes[i].burst_time;

    printf("Process ID\t\tRemaining Burst Time\n");
    while(remain>0)
    {
        for(int i = 0; i < n; i++)
        {
            if(remaining_burst[i]<TIME_QUANTUM)
            {
                remain--;
                remaining_burst[i] = 0;
                printf("Process[%d] is finish\n",processes[i].process_id);
            }
            else
            {
                remaining_burst[i]-=TIME_QUANTUM;
                if(remaining_burst[i] == 0)
                {
                    printf("Process[%d] is finish\n",processes[i].process_id);
                }
                else
                {
                    printf("Process[%d]\t\t\t%d\n", processes[i].process_id, remaining_burst[i]);
                }
            }
            
        }
    }
    
}

int main() {
    Process processes[] = {{1, 6}, {2, 8}, {3, 7}, {4, 3}};
    int n = sizeof(processes)/sizeof(processes[0]);
    RoundRobin(processes, n);
    return 0;
}
