#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

//Chanwoo Oh - 101118334
//Riya Arora - 101190033

// Define a struct for a queue node
typedef struct QueueNode {
	ProcessInfo process;
	struct QueueNode* next;
} QueueNode;

// Define a struct for a queue
typedef struct Queue {
	QueueNode* front;
	QueueNode* rear;
} Queue;



// Function to initialize a queue
Queue* initializeQueue() {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL) {
		perror("Error initializing queue");
		return NULL;
	}
	queue->front = NULL;
	queue->rear = NULL;
	return queue;
}

// Function to enqueue a process into the queue
void enqueue(Queue* queue, ProcessInfo process) {
	QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
	if (newNode == NULL) {
		perror("Error enqueuing process");
		return;
	}
	newNode->process = process;
	newNode->next = NULL;

	if (queue->rear == NULL) {
		// If the queue is empty, set both front and rear to the new node
		queue->front = newNode;
		queue->rear = newNode;
	} else {
		// Otherwise, add the new node to the rear of the queue
		queue->rear->next = newNode;
		queue->rear = newNode;
	}
}

// Function to dequeue a process from the queue
ProcessInfo dequeue(Queue* queue) {
	if (queue->front == NULL) {
		// Queue is empty, return an empty process
		ProcessInfo emptyProcess = {0}; // Assuming PID 0 is not used
		return emptyProcess;
	}

	// Get the front process
	QueueNode* frontNode = queue->front;
	ProcessInfo frontProcess = frontNode->process;

	// Move the front pointer to the next node
	queue->front = frontNode->next;

	// If the queue becomes empty, update the rear pointer
	if (queue->front == NULL) {
		queue->rear = NULL;
	}

	// Free the memory of the dequeued node
	free(frontNode);

	return frontProcess;
}


int isEmpty(Queue* queue) {
	return (queue->front == NULL);
}

// Function to free the memory used by a queue and its nodes
void freeQueue(Queue* queue) {
	if (queue == NULL) {
		return; // Return if the queue is already NULL
	}

	QueueNode* current = queue->front;
	while (current != NULL) {
		QueueNode* temp = current;
		current = current->next;
		free(temp);
	}

	free(queue); // Free the queue structure itself
}
void movingToWaiting (Queue* runnginQ, Queue* waitQ, int currentT, FILE *outputFile){
	fprintf(outputFile, "Process ID %d moved from RUNNING to WAITING at time %d\n",runnginQ->front->process.pid,currentT);
	runnginQ->front->process.io_frequency_remaining = runnginQ->front->process.io_frequency; //reset io frequency timer
	enqueue(waitQ,dequeue(runnginQ));

}

void movingToReady(Queue* waitingQ, Queue* readyQ, int currentT, FILE* outputFile) {
	QueueNode* current = waitingQ->front;
	QueueNode* previous = NULL;

	while (current != NULL) {
		current->process.io_duration_remaining--;
		if (current->process.io_duration_remaining == 0) {
			if (previous != NULL) {
				previous->next = current->next;
				current->next = NULL;

				// Reset the I/O duration timer
				current->process.io_duration_remaining = current->process.io_duration;
				enqueue(readyQ, current->process);
				fprintf(outputFile, "Process ID %d moved from WAITING to READY at time %d\n", current->process.pid, currentT);
				current = previous->next;

			} else {
				fprintf(outputFile, "Process ID %d moved from WAITING to READY at time %d\n", current->process.pid, currentT);
				current->process.io_duration_remaining = current->process.io_duration;
				enqueue(readyQ, current->process);
				// Reset the I/O duration timer

				dequeue(waitingQ);
				current = waitingQ->front;
			}
		} else {
			previous = current;
			current = current->next;
		}
	}
}



void movingToRunning (Queue* readyQ, Queue* RunningQ, int currentT, FILE *outputFile){
	fprintf(outputFile,"Process ID %d moved from READY to RUNNING at time %d\n",readyQ->front->process.pid,currentT);
	enqueue(RunningQ,dequeue(readyQ));
}

void movingToTerminating (Queue* runningQ, Queue* TerminatingQ, int currentT, FILE *outputFile){
	fprintf(outputFile,"Process ID %d moved from RUNNING to TERMINATING at time %d\n",runningQ->front->process.pid,currentT);
	enqueue(TerminatingQ,dequeue(runningQ));
}

// Function to simulate process state transitions
void simulate(ProcessInfo *processes, int numProcesses) {
	// Open an output file for writing
	FILE *outputFile = fopen("output.txt", "w");

	if (outputFile == NULL) {
		perror("Error opening output file");
		return;
	}
	// Initialize your queues and other variables
	fprintf(outputFile,"start of simulation with Processes: %d\n",numProcesses);
	Queue *newQueue = initializeQueue();
	Queue *readyQueue = initializeQueue();
	Queue *runningQueue = initializeQueue();
	Queue *terminatingQueue = initializeQueue();
	Queue *waitingQueue = initializeQueue();
	int currentTime = 0;
	int processed = 0;

	//put every process in newQueue
	for (int i =0;i<numProcesses;i++){
		enqueue(newQueue, processes[i]);

	}

	while(1){
		//admit to ready queue if process have arrived
		for (int i=0; i<numProcesses; i++){
			if (processes[i].arrival_time == currentTime){
				fprintf(outputFile,"Process ID %d moved from NEW to READY at time %d\n",processes[i].pid,currentTime);
				enqueue(readyQueue,dequeue(newQueue));
			}
		}
		//check if process finished waiting for i/o opearation
		if(!isEmpty(waitingQueue)){
			QueueNode* current = waitingQueue->front;
			QueueNode* previous = NULL;
			while (current != NULL) {
				current->process.io_duration_remaining--;
				if (current->process.io_duration_remaining == 0) {
					if (previous != NULL) {
						previous->next = current->next;
						current->next = NULL;

						// Reset the I/O duration timer
						current->process.io_duration_remaining = current->process.io_duration;
						enqueue(readyQueue, current->process);
						fprintf(outputFile, "Process ID %d moved from WAITING to READY at time %d\n", current->process.pid, currentTime);
						current = previous->next;

					} else {
						fprintf(outputFile, "Process ID %d moved from WAITING to READY at time %d\n", current->process.pid, currentTime);
						current->process.io_duration_remaining = current->process.io_duration;
						enqueue(readyQueue, current->process);
						// Reset the I/O duration timer

						dequeue(waitingQueue);
						current = waitingQueue->front;
					}
				} else {
					previous = current;
					current = current->next;
				}
			}
		}

		//check if a process is finished or need I/O operation in running Queue
		if (!isEmpty(runningQueue)){
			runningQueue->front->process.total_cpu_time--;
			runningQueue->front->process.io_frequency_remaining --;
			// for finished process
			if (runningQueue->front->process.total_cpu_time == 0){
				movingToTerminating(runningQueue,terminatingQueue,currentTime,outputFile);
				if(!isEmpty(readyQueue)){
					movingToRunning(readyQueue,runningQueue,currentTime,outputFile);
				}
				processed++;
			}
			// for i/o oepartion
			else if (runningQueue->front->process.io_frequency_remaining == 0){
				movingToWaiting(runningQueue,waitingQueue,currentTime,outputFile);
				if(!isEmpty(readyQueue)){
					movingToRunning(readyQueue,runningQueue,currentTime,outputFile);
				}
			}
		}

		//dispatch to running queue if process is ready and running queue is empty
		if(!isEmpty(readyQueue)&&isEmpty(runningQueue)){
			movingToRunning(readyQueue,runningQueue,currentTime,outputFile);
		}

		if (processed == numProcesses){
			break;
		}
		currentTime++;

		if (currentTime == 10000){
			printf("break because of TIME");
			break;
		}
	}
	freeQueue(newQueue);
	freeQueue(readyQueue);
	freeQueue(terminatingQueue);
	freeQueue(waitingQueue);
	freeQueue(runningQueue);
}


