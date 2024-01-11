/*
   Assignment 2 

   Author: Riya Arora
   Student #: 101190033
 */
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "singly_linked_list.h"

#define FCFS 1
#define PRIORITY 2
#define ROUND_ROBIN 3

typedef struct partition { // partition data structure
	int size;
	int space;
	    pcbnode_t * allocated_pcb;
} partition_t;

partition_t partition_construct(int size_) // partition data structure constructor
{
	partition_t p;
	p.size = size_;
	p.space = size_;
	p.allocated_pcb = NULL;
	return p;
}

partition_t main_memory[4]; // fixed partitioning

// prints the memory details such as partitions available, total free memory total available memory etc. 
void print_memory(void)
{
	int total_memory = main_memory[0].size + main_memory[1].size + main_memory[2].size + main_memory[3].size;
	int total_memory_available =
		main_memory[0].space + main_memory[1].space + main_memory[2].space + main_memory[3].space;
	int total_memory_usable = 0;
	//print memory metrics 
	printf("\n______________________________________________________\n");
	printf("TOTAL MEMORY USED: %d MB\n", total_memory - total_memory_available);
	for (int i = 0; i < 4; i++) {
		printf("\nPARTITION %d: ", i + 1);
		if (main_memory[i].allocated_pcb == NULL) {
			total_memory_usable += main_memory[i].size;
			printf("FREE\n");
		} else {
			printf("USED\n");
		}
	}

	printf("\n%d MB FREE MEMORY AVAILABLE\n", total_memory_available);
	printf("\nTOTAL MEMORY USABLE: %d MB\n", total_memory_usable);
	printf("\n______________________________________________________\n");

}

// memory management, only accesses if memory management is activated 
int memory_manage(pcb_t pcb, bool allocate)
{

	int address = 0;

	// Allocating memory 
	if (allocate) {

		for (int i = 0; i < 4; i++) {
			if (main_memory[i].allocated_pcb == NULL && pcb.size <= main_memory[i].size) { // partition is not being used and process can fit in the partition
				main_memory[i].space -= pcb.size;
				main_memory[i].allocated_pcb = pcbnode_construct(pcb, NULL);
				print_memory();
				return address; // start address for the PCB
			} else if (i == 3) {
				return -1;		// could not allocate memory (no usable space)
			}
			address += main_memory[i].size;
		}
	}
	// Deallocting memory 
	else {						
		// Find which partition the pcb is located in and add memeory back to that partition

		pcbnode_t* temp;
		for (int i = 0; i < 4; i++) {
			if (main_memory[i].allocated_pcb != NULL && pcb.pid == main_memory[i].allocated_pcb->pcb.pid) {	// check if the pcb is in the partition
				main_memory[i].space = main_memory[i].size;	// reset size of memory
				temp = main_memory[i].allocated_pcb;
				main_memory[i].allocated_pcb = NULL;	// deallocate from memory
				free(temp);
				print_memory();
				return 1;
			}
		}

	}
	return -1;					// error: pcb wasnt in the memory (shouldn't happen) 
}
// parse input file and generate a job_queue
pcbnode_t * load(char filename[])
{

	FILE * file = fopen(filename, "r");
	if (!file) {
		printf("Error opening file");
		return NULL;
	} else {
		char line[1024];		// Assuming max size of line to be 1000

		int row = 0;
		int column = 0;

		int pid = 0;
		int arrival_time = 0;
		int cpu_time = 0;
		int io_freq = 0;
		int io_dur = 0;
		int size = 0;
		int priority = 0;

		pcbnode_t * jq = NULL;

		while (fgets(line, 1024, file)) {
			column = 0;
			row++;

			char *data = strtok(line, ",");
			while (data) {

				// Ignore first row
				if (row == 1) ;

				// PID
				else if (column == 0) {
					pid = atoi(data);
					//printf("%s,", data);  
				}
				// Arrival Time
				else if (column == 1) {
					arrival_time = atoi(data);
					//printf("%s,", data);  
				}
				// Total CPU Time
				else if (column == 2) {
					cpu_time = atoi(data);
					//printf("%s,", data);
				}
				// I/O Frequency
				else if (column == 3) {
					io_freq = atoi(data);
					//printf("%s,", data);
				}
				// I/O Duration
				else if (column == 4) {
					io_dur = atoi(data);
					//printf("%s", data);
				}
				// Size
				else if (column == 5) {
					size = atoi(data);
				}
				// Priority
				else if (column == 6) {
					priority = atoi(data);
				}

				data = strtok(NULL, ",");
				column++;
			}
			if (row != 1) {
				if (size <= main_memory[0].size || size <= main_memory[1].size || size <= main_memory[2].size
					|| size <= main_memory[3].size) {
					jq = append(jq, pcb_construct(pid, arrival_time, cpu_time, io_freq, io_dur, size, -1, -1,
							priority));
				} else {
					printf("\nError loading PCB %d: Size of memeory exceeds a partition\n", pid); // error check for a process greater than the largest partition
				}
			}
		}

		fclose(file);
		return jq;
	}
}

// Main simulation 
void simulate(int scheduler, int mem_manage, char filein[], char fileout[], int timer)
{

	// Load input
	pcbnode_t * job_queue = load(filein);

	// Create output file
	FILE * output_file = fopen(fileout, "w+");

	// initialize all of the scheduling queues / required pointers  
	pcbnode_t * curr = NULL;
	pcbnode_t * prev = NULL;
	pcbnode_t * min_priority;
	pcbnode_t * min_priority_prev;
	pcbnode_t * ready_queue = NULL;
	pcbnode_t * running = NULL;
	pcbnode_t * waiting = NULL;
	pcbnode_t * temp = NULL;
	int TIMEOUT = timer;
	int time = 0;

	// Scheduling metrics
	unsigned int num_process = 0;
	unsigned int cpu_time = 0;
	unsigned int waiting_time = 0;
	unsigned int turn_around_time = 0;
	unsigned int response_time = 0;

	fprintf(output_file, "Time of transition,PID,Old State,New State\n");

	//loop that terminates once all the queues in the kernel simulator are empty
	while (job_queue != NULL || ready_queue != NULL || running != NULL || waiting != NULL) {

		// if a process is running, decrement its cpu time remaining, check if it has an I/O system call block  
		if (running != NULL) {
			running->pcb.remaining_CPU_time--;
			cpu_time++;			// increase cpu time metric
			timer--;

			// RUNNING -> TERMINATED
			if (running->pcb.remaining_CPU_time == 0) {
				fprintf(output_file, "%d,%d,RUNNING,TERMINATED\n", time, running->pcb.pid);
				turn_around_time += time - running->pcb.arrival_time;	// Calculate turn around time right before termination
				if ((scheduler == ROUND_ROBIN || scheduler == FCFS) && mem_manage == 1) {
					memory_manage(running->pcb, false);	// deallocate from memory (only in FCFS or RR) 
				}
				free(running);
				running = NULL;
				num_process++;	// increase number of processes metric 
				timer = TIMEOUT;	// resert timer (for RR)
			}
			// RUNNING -> WAITING
			else if ((running->pcb.total_CPU_time - running->pcb.remaining_CPU_time) % running->pcb.io_freq == 0
				&& running->pcb.io_freq <= running->pcb.total_CPU_time) {
				waiting = append(waiting, pop(running)->pcb);
				fprintf(output_file, "%d,%d,RUNNING,WAITING\n", time, running->pcb.pid);
				free(running);
				running = NULL;
				timer = TIMEOUT;	// reset timer (for RR)
			}
		}

		curr = job_queue;
		prev = NULL;

		// NEW -> READY
		// go through job queue too see if arrival time == current time then move to ready queue
		while (curr != NULL && curr->pcb.arrival_time <= time) {
			if (curr->pcb.start_address == -1 && mem_manage == 1) {
				curr->pcb.start_address = memory_manage(curr->pcb, true);	// try to allocate in main memory 
				curr->pcb.end_address = curr->pcb.start_address + curr->pcb.size;
			}
			if (mem_manage == 0 || curr->pcb.start_address != -1) {
				ready_queue = append(ready_queue, curr->pcb);
				fprintf(output_file, "%d,%d,NEW,READY\n", time, curr->pcb.pid);

				if (prev == NULL) {	// removing first element will always occur if mem_manage is 0 (inactve)
					temp = job_queue;
					job_queue = job_queue->next;
					free(temp);
					curr = job_queue;
				}

				else {			// removing middle or last element
					temp = curr;
					prev->next = curr->next;
					curr->next = NULL;
					free(temp);
					curr = prev->next;
				}

			} else {
				prev = curr;
				curr = curr->next;
			}
		}

		if (running != NULL) {
			// RUNNING -> READY (Round Robin)
			// must be done after new->ready to allow for new processes to execute first
			if (scheduler == ROUND_ROBIN && timer == 0) {
				ready_queue = append(ready_queue, pop(running)->pcb);
				fprintf(output_file, "%d,%d,RUNNING,READY\n", time, running->pcb.pid);
				free(running);
				running = NULL;
				timer = TIMEOUT;	// reset timer
			}
		}

		// WAITING -> READY
		// Check all processes in waiting,move to ready if I/O duration is 0,decrement remaining I/O durations,

		while (waiting != NULL && waiting->pcb.remaining_io_dur == 0)	// Check and move first pcb in waiting queue, repeat if required
		{
			waiting->pcb.remaining_io_dur = waiting->pcb.io_dur;
			ready_queue = append(ready_queue, pop(waiting)->pcb);
			fprintf(output_file, "%d,%d,WAITING,READY\n", time, waiting->pcb.pid);
			temp = waiting;
			waiting = waiting->next;
			free(temp);

		}

		curr = waiting;
		prev;
		while (curr != NULL)	// Check remaining nodes in waiting queue move to ready if required
		{
			prev = curr;
			curr = curr->next;

			if (curr != NULL && curr->pcb.remaining_io_dur == 0) {
				curr->pcb.remaining_io_dur = curr->pcb.io_dur;
				ready_queue = append(ready_queue, pop(curr)->pcb);
				fprintf(output_file, "%d,%d,WAITING,READY\n", time, curr->pcb.pid);
				prev->next = curr->next;
				temp = curr;
				curr = waiting;
				free(temp);
			}
		}

		curr = waiting;
		while (curr != NULL)	// Decrement I/O duration for all elements still in waiting queue
		{
			curr->pcb.remaining_io_dur--;
			curr = curr->next;
		}

		// READY -> RUNNING
		// check if a process is running if not add it from ready queue 
		if (running == NULL && ready_queue != NULL) {
			if (scheduler != PRIORITY) {	// use FCFS algorithm or ROUND_ROBIN both are same for ready -> running 
				if (ready_queue->pcb.remaining_CPU_time == ready_queue->pcb.total_CPU_time) {
					response_time += time - ready_queue->pcb.arrival_time;	// Add to response time if its the first response of process
				}
				running = pop(ready_queue);
				fprintf(output_file, "%d,%d,READY,RUNNING\n", time, running->pcb.pid);
				temp = ready_queue;
				ready_queue = ready_queue->next;
				free(temp);

			} else { // Priority scheduling
				min_priority = ready_queue;
				min_priority_prev = NULL;
				curr = ready_queue;
				prev = NULL;

				while (curr != NULL) {
					if (curr->pcb.priority < min_priority->pcb.priority) {
						min_priority = curr;
						min_priority_prev = prev;
					}
					prev = curr;
					curr = curr->next;
				}

				if (min_priority->pcb.remaining_CPU_time == min_priority->pcb.total_CPU_time) {
					response_time += time - min_priority->pcb.arrival_time;	// Add to response time if its the first response of process
				}
				running = pcbnode_construct(min_priority->pcb, NULL);
				fprintf(output_file, "%d,%d,READY,RUNNING\n", time, running->pcb.pid);

				if (min_priority_prev == NULL) {	// removing first element
					temp = ready_queue;
					ready_queue = ready_queue->next;
					free(temp);
				} else {		// removing middle or last element
					temp = min_priority;
					min_priority_prev->next = min_priority->next;
					min_priority->next = NULL;
					free(temp);
				}
			}
		}

		// increase waiting time metric
		curr = ready_queue;
		while (ready_queue != NULL && curr != NULL) {
			curr = curr->next;
			waiting_time++;
		}

		time++;
	}

	fclose(output_file);

	printf("\n***Output file created***\n");

	//Print metrics to terminal
	printf("\n***Simulation Metrics***\n");
	printf("Average CPU Utilization: %.1f%%\n", (float)cpu_time / (time - 1) * 100);
	printf("Throughput: %.1f jobs/s\n", (float)num_process / time * 1000);
	printf("Average Turn Around Time: %.1f ms\n", (float)turn_around_time / num_process);
	printf("Average Waiting Time: %.1f ms\n", (float)waiting_time / num_process);
	printf("Average Response Time: %.1f ms\n", (float)response_time / num_process);

}

int main(int argc, char *argv[])
{
	printf("\nSCHEDULER: ");
	if (atoi(argv[1]) == FCFS) {
		printf("FCFS\n");

	} else if (atoi(argv[1]) == PRIORITY) {
		printf("PRIORITY\n");

	} else {
		printf("ROUND ROBIN\n");
	}
	// Construct main memory parititons
	main_memory[0] = partition_construct(atoi(argv[5]));
	main_memory[1] = partition_construct(atoi(argv[6]));
	main_memory[2] = partition_construct(atoi(argv[7]));
	main_memory[3] = partition_construct(atoi(argv[8]));

	simulate(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4], atoi(argv[9]));
	printf("\n***END OF SIMULATION***\n");
	return 0;
}
