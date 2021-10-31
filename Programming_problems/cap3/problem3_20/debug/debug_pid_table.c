#include "../pid_table.h"
#include "../bit_array.h"
#include <stdlib.h>
#include <stdio.h>

static void print_pid_table(pid_table *target_pid_table);


int main(void)
{
	pid_table *my_pid_table;
	int pid_allocated, alloc_pid_status, i;

	printf("Create a pid table with pid range of [-5, 6]\n");
	my_pid_table = create_pid_table(-5, 6);
	print_pid_table(my_pid_table);
	printf("\n");
	printf("Assign 4 pids.\n");
	for(i = 0; i < 4; i++)
	{
		alloc_pid_status = alloc_min_pid_available(my_pid_table, &pid_allocated);
		if(alloc_pid_status == 1) printf("PID %d allocated successfully.\n", pid_allocated);
		else if(alloc_pid_status == 0) printf("TABLE IS FULL!\n");
		else printf("ERROR.\n");
	}
	print_pid_table(my_pid_table);
	printf("\n");
	printf("Free pid -5\n");
	if(free_pid(my_pid_table, -5) == 1) printf("Success\n");
	else printf("Fail\n");
	printf("Free pid -10\n");
	if(free_pid(my_pid_table, -10) == 1) printf("Success\n");
	else printf("Fail\n");
	printf("Free pid -3\n");
	if(free_pid(my_pid_table, -3) == 1) printf("Success\n");
	else printf("Fail\n");
	printf("Free pid -6\n");
	if(free_pid(my_pid_table, -6) == 1) printf("Success\n");
	else printf("Fail\n");
	print_pid_table(my_pid_table);
	printf("\n");

	printf("Assign 12 pids.\n");
	for(i = 0; i < 12; i++)
	{
		alloc_pid_status = alloc_min_pid_available(my_pid_table, &pid_allocated);
		if(alloc_pid_status == 1) printf("PID %d allocated successfully.\n", pid_allocated);
		else if(alloc_pid_status == 0) printf("TABLE IS FULL!\n");
		else printf("ERROR.\n");
	}
	print_pid_table(my_pid_table);
	printf("\n");
	

	//Delete pid table:
	delete_pid_table(&my_pid_table);

	return 0;
}


static void print_pid_table(pid_table *target_pid_table)
{
	int current_pid;
	int min_pid, max_pid;
	int check_pid_status;
	printf("PID TABLE: \n");
	if(!target_pid_table) printf("\tNULL\n");
	else
	{
		min_pid = target_pid_table->min_pid;
		max_pid = target_pid_table->max_pid;
		printf("\t- Size: %d\n", max_pid - min_pid + 1);
		printf("\t- Number of PID availables: %d\n", get_bit_array_size(target_pid_table->pid_bit_array) - get_num_of_set_bits(target_pid_table->pid_bit_array));
		printf("\t- List of PIDS:\n");
		current_pid = min_pid;
		for(current_pid = min_pid; current_pid <= max_pid; current_pid++)
		{
			check_pid_status = check_pid(target_pid_table, current_pid);
			printf("\t\t %+4d ", current_pid);
			if(check_pid_status == 0) printf(" (NOT AVAILABLE)\n");
			else if(check_pid_status == 1)printf(" (AVAILABLE)\n");
			else printf(" (ERROR)\n");
		}
	}
}

