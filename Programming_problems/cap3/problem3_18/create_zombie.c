#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main(void)
{
	pid_t pid;
	const int waiting_time = 10;
	unsigned int remaining_time_to_sleep = waiting_time;

	pid = fork();

	if(pid == 0)//Child
	{
		printf("CHILD: I am going to die.\n");
	}
	else if(pid > 0) //Parent
	{
		printf("PARENT: I will wait for %d s.\n", waiting_time);
		while(remaining_time_to_sleep > 0) remaining_time_to_sleep = sleep(remaining_time_to_sleep);
		printf("PARENT: I am going to die.\n");
	}
	else //Error
	{
		perror("Error while forking the process.\n");
	}


	return 0;
}
