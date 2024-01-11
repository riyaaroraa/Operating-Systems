// process.c

//Chanwoo Oh - 101118334
//Riya Arora - 101190033

#include <stdio.h>
#include <stdlib.h>
#include "process.h"

// Function to count the number of processes in the CSV file, excluding the header row
int countProcessesInFile(const char *filename) {
	FILE *file = fopen(filename, "r"); // Open the CSV file for reading

	if (file == NULL) {
		perror("Error opening file");
		return -1; // Return -1 to indicate an error
	}

	int numProcesses = 0; // Initialize the count to 0
	char line[1024]; // Assuming a maximum line length of 1024 characters

	// Skip the header row (first row)
	if (fgets(line, sizeof(line), file) == NULL) {
		fclose(file);
		return -1; // Error reading the header row
	}

	// Read data from the CSV file and increment the count for each valid line
	while (fgets(line, sizeof(line), file) != NULL) {
		numProcesses++; // Increment the number of processes read
	}

	fclose(file); // Close the file when done

	return numProcesses; // Return the number of processes found in the file (excluding the header)
}




ProcessInfo* readProcessesFromFile(const char *filename, int numProcesses) {
	if (numProcesses == -1) {
		return NULL; // Error in counting processes
	}

	FILE *file = fopen(filename, "r"); // Open the CSV file for reading

	if (file == NULL) {
		perror("Error opening file");
		return NULL; // Return NULL to indicate an error
	}

	ProcessInfo *processes = malloc(numProcesses * sizeof(ProcessInfo)); // Allocate memory

	if (processes == NULL) {
		perror("Error allocating memory");
		fclose(file);
		return NULL; // Return NULL to indicate an error
	}

	char line[1024]; // Assuming a maximum line length of 1024 characters

	// Skip the header row (first row)
	if (fgets(line, sizeof(line), file) == NULL) {
		fclose(file);
		free(processes);
		return NULL; // Error reading the header row
	}

	int i = 0;
	// Read data from the CSV file and assign it to the struct, starting from row 2
	while (fgets(line, sizeof(line), file) != NULL) {
		// Parse the values from the CSV line and assign them to the struct
		if (sscanf(line, "%d,%d,%d,%d,%d",
				&processes[i].pid,
				&processes[i].arrival_time,
				&processes[i].total_cpu_time,
				&processes[i].io_frequency,
				&processes[i].io_duration) != 5) {
			perror("Error reading data from file");
			fclose(file);
			free(processes);
			return NULL; // Return NULL to indicate an error
		}
		processes[i].io_frequency_remaining = processes[i].io_frequency;
		processes[i].io_duration_remaining = processes[i].io_duration;
		i++; // Move to the next process
	}

	fclose(file); // Close the file when done

	return processes; // Return the array of processes
}



