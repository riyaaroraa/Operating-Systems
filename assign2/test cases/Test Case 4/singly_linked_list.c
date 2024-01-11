/*    Assignment 2 

   Author: Riya Arora
   Student #: 101190033
 */

#include <assert.h>  // assert
#include <stdlib.h>  // malloc, free
#include <stdbool.h>
#include <stdio.h>   // printf

#include "singly_linked_list.h"



pcb_t pcb_construct(int pid_, int arrival_time_, int cpu_time_, int io_freq_, int io_dur_, int size_, int start_address_, int end_address_ ,int priority_)
{
	pcb_t p;
	p.pid = pid_;
    p.arrival_time = arrival_time_;
    p.total_CPU_time = cpu_time_;
	p.remaining_CPU_time = cpu_time_;
	p.io_freq = io_freq_;
	p.io_dur = io_dur_;
	p.remaining_io_dur = io_dur_;
	p.size = size_;
	p.start_address = start_address_;
	p.end_address = end_address_;
	p.priority = priority_;
    return p;
}

/* Return a pointer to a new pcbnode_t (a node in a singly-linked list).

   Parameter value: the pcb to be stored in the node.
   Parameter next: a pointer to the node that the new node should point to.
   This pointer should be NULL if the new node is the last node in the 
   linked list.
   Terminate (via assert) if memory for the node cannot be allocated.
 */ 
pcbnode_t *pcbnode_construct(pcb_t value, pcbnode_t *next)
{
    pcbnode_t *p = malloc(sizeof(pcbnode_t));
    assert (p != NULL);
    p->pcb = value;
    p->next = next;
    return p;
}
/* Parameter head points to the first node in a linked list, or is NULL
   if the linked list is empty. Insert a node containing the specified 
   integer value at the of the linked list, and return a pointer to 
   the first node in the modified list.
 */
pcbnode_t *push(pcbnode_t *head, pcb_t value)
{
    return pcbnode_construct(value, head);
}

/* Parameter head points to the first node in the list.
   Return the length of a linked list (0 if the linked list is empty). 
 */
int length(pcbnode_t *head)
{
    int count = 0;
    for (pcbnode_t * curr = head; curr != NULL; curr = curr->next) {
        count += 1;
    }
    return count;
}

/* Parameter head points to the first node in a linked list, or is
 * NULL if the list is empty.
 *
 * The function terminates (via assert) if the linked list is empty.
 *
 * Return and remove the first value in the linked list 
 */

pcbnode_t *pop(pcbnode_t *head)
{
	assert(head != NULL);
    pcb_t popped_value = head->pcb;
	head = head->next;
    return pcbnode_construct(popped_value, NULL);
}

/* parameter head points to the first pcb node in a linked list, or is 
 * NULL if the list is empty. 
 * funtion returns the head of a linked list with a new node inserted
 * at the end.
 */

pcbnode_t* append(pcbnode_t* head, pcb_t data)
{
   	pcbnode_t* new_node = pcbnode_construct(data, NULL);  
     /* 4. If the Linked List is empty, then make the new node as head */
    if (head == NULL)
    {
       head = new_node;
       return head;
    } 
	else if(head->next == NULL){
		head->next = new_node;
		return head;
	} else{
      	pcbnode_t* curr = head;

 	  	while (curr->next != NULL){
        	curr = curr->next;
  		}
  	  	curr->next = new_node;
 	   	return head;   
	}
}
/* Parameter head points to the first node in a linked list.
   Print the linked list, using the format:
       value1 -> value2 -> value3 -> ... -> last_value
*/
 
void print_list(pcbnode_t *head)
{
    if (head == NULL) {
        printf("empty list");
        return; 
    }

    pcbnode_t *curr;

    /* The loop prints a every node in the linked list except the last one,
       using the format "value -> ", where "->" represents the link from each node
       to the following node.

       Notice that the loop condition is:
           curr->next != NULL
       and not:
           curr != NULL

       During the last iteration, the value in the second-last node is
       printed, then curr is updated to point to the last node. The
       condition curr->next != NULL now evaluates to false, so the 
       loop exits, with curr pointing to the last node (which has  
       not yet been printed.)*/
     

    for (curr = head; curr->next != NULL; curr = curr->next) {
        printf("%d -> ", curr->pcb.pid);
    }

    /* Print the last node. */
    printf("%d", curr->pcb.pid);
}

