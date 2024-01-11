Author: Riya Arora (101190033) & Chanwoo Oh (101118334)

In this folder you will find an executable (Scheuduler.exe) the source code
(main.c, singly_linked_list.c) and a header file (singly_linked_list.h).
These three files are required for the program to compile and execute. 

A copy of these files will be found in each test case folder to allow the batch
files to function. Each test case folder also consists of a test case '.csv' file,
the output '.csv' file and a batch '.bat' file to automate the execution. 
---------------------------------------------------------------------------------------------------------
HOW TO USE: 

double-click on the .bat file in any test case to view the output on the terminal and excel should open the output.csv files. 

---------------------------------------------------------------------------------------------------------

THE BATCH FILES:

The arguements for the executable are structured as so, 

"Scheduler.exe arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8 arg9"
equivalent to,
"Scheduler.exe (scheduler) (memory management) (Inputfile.csv) (Outputfile.csv) (partition 1 size (MB)) (partition 2 size (MB)) (partition 3 size (MB)) (partition 4 size (MB)) (Time quantum)"  


with 9 arguemnts for the executable:

arg1: 1 = FCFS, 2 = PRIORITY, 3 = ROUND ROBIN

arg2: memory management? 0 = yes, 1 = no (memory is ignored)

arg3: name of the input file, must include ".csv"

arg4: name of the output file, must include ".csv" 

arg5 - arg8: size of the parititons in MB must be positive need to be included even if memory is ignored. 4 partition minimum and maximum. 

arg9: length of time for the time quantum in ms. Must include even if RR scheduling is not being used

-----------------------------------------------------------------------------------------------------------
THE INPUT FILES:

The test case input file must be structured as so in a .csv format for the program to work,

Pid, Arrival Time, Total CPU Time, I/O Frequency, I/O Duration, Size (MB), Priority

NOTE: Arrival time must be sorted in order from least to greatest for program to function correctly.
Process size should be 0 if memory is not being used to avoid initialization error.

-----------------------------------------------------------------------------------------------------------

MEMORY:

Memory uses a first fit policy. 

If a processes memory is larger than the largest partition size it will be ignored and a message will be outputed to the terminal.

 



