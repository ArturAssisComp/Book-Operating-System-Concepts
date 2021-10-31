#ifndef PID_TABLE_H
#define PID_TABLE_H

#include "bit_array.h"

//Types:
typedef struct
{
	bit_array *pid_bit_array;
	int min_pid;
	int max_pid;
	int min_pid_available;
}pid_table;

//Function declarations:
pid_table *create_pid_table(int min_pid, int max_pid);
void delete_pid_table(pid_table **target_pid_table_address);
int check_pid(pid_table *target_pid_table, int pid);
int free_pid(pid_table *target_pid_table, int pid);
int alloc_min_pid_available(pid_table *target_pid_table, int *pid_allocated_address);
#endif
