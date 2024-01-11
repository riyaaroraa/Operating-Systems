/* A node in a singly-linked list that stores values of type pcb. 

Author: Riya Arora (101190033) & Chanwoo Oh (101118334)

*/

typedef struct pcb {
    int pid;
    int arrival_time; 
    int total_CPU_time;
    int remaining_CPU_time;
    int io_freq;
    int io_dur;
	int remaining_io_dur;
	int size;
	int start_address;
	int end_address;
	int priority;
} pcb_t;

typedef struct pcbnode {
    pcb_t pcb;
    struct pcbnode *next;
} pcbnode_t;

pcb_t pcb_construct(int pid_, int arrival_time_, int cpu_time_, int io_freq_, int io_dur_, int size_, int start_address_, int end_address_ ,int priority_);
pcbnode_t *pcbnode_construct(pcb_t value, pcbnode_t *next);
pcbnode_t *push(pcbnode_t *head, pcb_t value);
int length(pcbnode_t *head);
pcbnode_t* append(struct pcbnode* head, pcb_t data);

void print_list(pcbnode_t *head);
pcbnode_t *pop(pcbnode_t *head);
