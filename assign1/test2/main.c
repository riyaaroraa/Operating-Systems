#include <stdio.h>
#include <stdlib.h>
#include "process.h"

//Chanwoo Oh - 101118334
//Riya Arora - 101190033

int main() {
	const char *filename = "test_case_2.csv"; // Replace with your CSV file name
	int numProcesses = countProcessesInFile(filename);
	//printf("Number of processes in the file: %d\n", numProcesses); // Print the number of processes
	if (numProcesses == -1) {
		printf("Error counting processes in the file.\n");
		return 1;
	}

	ProcessInfo *processesFromCSV = readProcessesFromFile(filename, numProcesses);

	if (processesFromCSV == NULL) {
		printf("Error reading the file or allocating memory.\n");
		return 1;
	}

	// Run the simulation
	simulate(processesFromCSV,numProcesses);
	// Free the dynamically allocated memory when you're done
	free(processesFromCSV);
	return 0;
}
