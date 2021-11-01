#include "pid_manager.h"
#include <stdio.h>
#include <stdlib.h>


pid_table *global_pid_table = NULL;

//Function definitions:
int allocate_map(void)
/**
 * Description: Create and initializes a data structure for representing pids;
 * returns -1 if unsuccessful, 1 if successful.
 */
{

	global_pid_table = create_pid_table(MIN_PID, MAX_PID);
	if(global_pid_table) return 1;
	else return -1;
}

void free_map(void)
/**
 * Description: This function deletes the data structure created in the function 'allocate_map'. 
 */
{

	delete_pid_table(&global_pid_table);
}


int allocate_pid(void)
/**
 * Description: Allocates and returns a PID; returns -1 if unable to allocate a 
 * pid (all pids are in use);
 */
{
	int allocated_pid;
	int alloc_status;

	alloc_status = alloc_min_pid_available(global_pid_table, &allocated_pid);

	if(alloc_status == 0) return -1; //Table is full
	else if(alloc_status == 1) return allocated_pid;
	else 
	{
		fprintf(stderr, "allocate_pid: pid table corrupted.\n");
		exit(EXIT_FAILURE);
	}

}
void release_pid(int pid)
//Releases a pid;
{
	int free_pid_status;

	if(free_pid(global_pid_table, pid) == -1)
	{
		fprintf(stderr, "release_pid: can not free the desired pid.\n");
		exit(EXIT_FAILURE);
	}
}

