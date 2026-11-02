
#include <stdio.h>

#define MAX 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int completed;
} Process;

void printResults(Process p[], int n, int order[]) {
    float totalWT = 0, totalTAT = 0, totalRT = 0;

    printf("\nGantt Chart: |");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", order[i]);
    }
    printf("\n\n");

    printf("PID\tAT\tBT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].waiting_time,
               p[i].turnaround_time,
               p[i].response_time);

        totalWT += p[i].waiting_time;
        totalTAT += p[i].turnaround_time;
        totalRT += p[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", totalWT / n);
    printf("Average Turnaround Time: %.2f\n", totalTAT / n);
    printf("Average Response Time: %.2f\n", totalRT / n);
}

void fcfs(Process p[], int n) {
    int order[MAX];
    int time = 0;

    printf("\n=== First Come First Served (FCFS) ===\n");

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[j].arrival_time < p[i].arrival_time) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival_time) {
            time = p[i].arrival_time;
        }

        p[i].waiting_time = time - p[i].arrival_time;
        p[i].response_time = p[i].waiting_time;

        time += p[i].burst_time;

        p[i].turnaround_time = time - p[i].arrival_time;
        order[i] = p[i].pid;
    }

    printResults(p, n, order);
}

void sjf(Process p[], int n) {
    int time = 0;
    int count = 0;
    int order[MAX];

    printf("\n=== Shortest Job First (SJF) ===\n");

    while (count < n) {
        int shortest = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].completed == 0 &&
                p[i].arrival_time <= time) {

                if (shortest == -1 ||
                    p[i].burst_time < p[shortest].burst_time ||
                    (p[i].burst_time == p[shortest].burst_time &&
                     p[i].arrival_time < p[shortest].arrival_time)) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        p[shortest].waiting_time =
            time - p[shortest].arrival_time;

        p[shortest].response_time =
            p[shortest].waiting_time;

        time += p[shortest].burst_time;

        p[shortest].turnaround_time =
            time - p[shortest].arrival_time;

        p[shortest].completed = 1;
        order[count] = p[shortest].pid;

        count++;
    }

    printResults(p, n, order);
}

int main() {
    Process processes[MAX];
    Process fcfsProcesses[MAX];
    Process sjfProcesses[MAX];
    int n;

    printf("Enter the number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;

        printf("Enter arrival time and burst time for process %d: ",
               i + 1);

        if (scanf("%d %d",
                  &processes[i].arrival_time,
                  &processes[i].burst_time) != 2 ||
            processes[i].arrival_time < 0 ||
            processes[i].burst_time <= 0) {
            printf("Invalid arrival or burst time.\n");
            return 1;
        }

        processes[i].completed = 0;

        fcfsProcesses[i] = processes[i];
        sjfProcesses[i] = processes[i];
    }

    fcfs(fcfsProcesses, n);
    sjf(sjfProcesses, n);

    return 0;
}
