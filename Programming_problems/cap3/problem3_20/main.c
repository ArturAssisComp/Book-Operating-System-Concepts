#include "pid_manager.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	int pid_allocated, i;

	if(allocate_map() == 1) printf("PID manager initialized successfully.\n");
	else
	{
		fprintf(stderr, "Error while initializing the PID manager.\n");
		exit(EXIT_FAILURE);
	}


	printf("Allocate 4 pids for testing:\n");
	for(i = 0; i < 4; i++)
	{
		pid_allocated = allocate_pid();
		if(pid_allocated == -1) printf("All pids are in use.\n");
		else printf("pid %d allocated.\n", pid_allocated);
	}

	printf("\n");
	printf("Release 2 of them\n");
	printf("Release pid 302\n");
	release_pid(302);
	printf("Release pid 300\n");
	release_pid(300);
	
	printf("\n");
	printf("Allocate 4 pids again:\n");
	for(i = 0; i < 4; i++)
	{
		pid_allocated = allocate_pid();
		if(pid_allocated == -1) printf("All pids are in use.\n");
		else printf("pid %d allocated.\n", pid_allocated);
	}

	free_map();

	return 0;

}
