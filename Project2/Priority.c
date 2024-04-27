#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int process_id;
    int burst_time;
    int priority;
} Process;

void SJF(Process processes[], int n) {
    int i, j, pos, temp;
    float total = 0, avg_wt, avg_tat;
    int *wait_time = malloc(sizeof(int) * n);
    int *turnaround_time = malloc(sizeof(int) * n);

    // 根據 burst_time 對進程進行排序
    for (i = 0; i < n; i++) {
        pos = i;
        for (j = i + 1; j < n; j++) {
            if (processes[j].priority < processes[pos].priority) {
                pos = j;
            }
        }

        // 交換位置
        temp = processes[i].burst_time;
        processes[i].burst_time = processes[pos].burst_time;
        processes[pos].burst_time = temp;

        temp = processes[i].process_id;
        processes[i].process_id = processes[pos].process_id;
        processes[pos].process_id = temp;

        temp = processes[i].priority;
        processes[i].priority= processes[pos].priority;
        processes[pos].priority = temp;
    }

    // 計算等待時間和周轉時間
    wait_time[0] = 0; // 第一個進程的等待時間為0

    for (i = 1; i < n; i++) {
        wait_time[i] = 0;
        for (j = 0; j < i; j++)
            wait_time[i] += processes[j].burst_time;

        total += wait_time[i];
    }

    avg_wt = total / n; // 平均等待時間
    total = 0;

    printf("\nProcess ID\tBurst Time\tPriority\tWaiting Time\tTurnaround Time");
    for (i = 0; i < n; i++) {
        turnaround_time[i] = processes[i].burst_time + wait_time[i]; // 周轉時間
        total += turnaround_time[i];
        printf("\n%d\t\t%d\t\t%d\t\t%d\t\t%d", processes[i].process_id, processes[i].burst_time,processes[i].priority, wait_time[i], turnaround_time[i]);
    }

    avg_tat = total / n; // 平均周轉時間
    printf("\n\nAverage Waiting Time = %.2f", avg_wt);
    printf("\nAverage Turnaround Time = %.2f\n", avg_tat);
}

int main() {
    Process processes[] = {{1, 6,3}, {2, 8,1}, {3, 7,2}, {4, 3,4}};
    int n = sizeof(processes)/sizeof(processes[0]);
    SJF(processes, n);
    return 0;
}
