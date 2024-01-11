// process.h

#ifndef PROCESS_H
#define PROCESS_H

//Chanwoo Oh - 101118334
//Riya Arora - 101190033

// Define the struct for the process
typedef struct {
    int pid;
    int arrival_time;
    int total_cpu_time;
    int io_frequency;
    int io_duration;
    int io_frequency_remaining;
    int io_duration_remaining;
    char process_old_state[20];
    char process_new_state[20];
} ProcessInfo;

int countProcessesInFile(const char *filename);
ProcessInfo* readProcessesFromFile(const char *filename, int numProcesses);
void simulate(ProcessInfo *processes, int numProcesses);
#endif
