#include <stdio.h>

// 定義進程結構
typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
} Process;

// 函數原型聲明
void FCFS(Process processes[], int n);

// 主函數
int main(int argc, char **argv) {
    Process processes[] = {{1, 0, 10}, {2, 1, 5}, {3, 2, 8}};
    int n = sizeof(processes) / sizeof(processes[0]);
    FCFS(processes, n);
    return 0;
}

void FCFS(Process processes[], int n) {
    int wait_time = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;

    printf("Process ID\tBurst Time\tWait Time\tTurnaround Time\n");

    for (int i = 0; i < n; i++) {
        int turnaround_time = wait_time + processes[i].burst_time;
        printf("%d\t\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].burst_time, wait_time, turnaround_time);

        total_wait_time += wait_time;
        total_turnaround_time += turnaround_time;
        wait_time += processes[i].burst_time;
    }

    printf("\nAverage Waiting Time = %.2f\n", (float)total_wait_time / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_turnaround_time / n);
}
