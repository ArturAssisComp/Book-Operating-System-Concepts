#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shm_conf.h"



int main(int argc, char *argv[])
{
	int shared_memory_fd, child_process_status;
	char *command;
	char *new_argv[] = {NULL, NULL};
	char *new_env[] = {NULL};
	pid_t pid;
	const char shared_memory_name[] = "/shared_memory__time_function";
	shm_buffer *shm_pointer;
	struct timeval final_timestamp, delta_time;
	double seconds_elapsed;

	if(argc != 2)
	{
		fprintf(stderr, "Correct use: %s <command>\n", argv[0]);
		goto error;
	}
	else
	{
		command = argv[1];
		//-------------------------------------------------------------
		//Create the shared memory region:
		shared_memory_fd = shm_open(shared_memory_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

		if (shared_memory_fd == -1)
		{
			perror("shm_open");
			goto error;
		}
		//Configure the size of the shared memory region:
		if(ftruncate(shared_memory_fd, sizeof(shm_buffer)) == -1) 
		{
			perror("ftruncate");
			goto error;
		}

		//Map the shared memory region:
		shm_pointer = mmap(NULL, sizeof *shm_pointer, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
		if(shm_pointer == MAP_FAILED) 
		{
			perror("mmap");
			goto error;
		}
		shm_pointer->time_updated = 0;

		//Close fd:
		if(close(shared_memory_fd) == -1)
		{
			perror("close");
			goto error;
		}

		//-------------------------------------------------------------

		pid = fork();
		if(pid == 0)//Child
		{
			//-------------------------------------------------------------
			//Get the start timestamp:
			if(gettimeofday(&shm_pointer->start_timestamp, NULL) == -1)
			{
				perror("gettimeofday");
				goto error;
			}

			shm_pointer->time_updated = 1;

			//-------------------------------------------------------------
			//Execute the command:
			new_argv[0] = command;
			execve(command, new_argv, new_env);
			perror("execve");
			goto error;

			//-------------------------------------------------------------

		}
		else if (pid > 0)//Parent
		{
			//-------------------------------------------------------------
			//Wait for the child process to finish:
			if (wait(&child_process_status) == -1)
			{
				perror("wait");
				goto error;
			}

			if(child_process_status != 0 || shm_pointer->time_updated == 0)
			{
				fprintf(stderr, "Some error occured during the execution of the child process.\n");
				goto error;
			}

			//-------------------------------------------------------------
			//Get the end timestamp:
			if(gettimeofday(&final_timestamp, NULL) == -1)
			{
				perror("gettimeofday");
				goto error;
			}

			//-------------------------------------------------------------
			//Unlink the shared memory:
			if (shm_unlink(shared_memory_name) == -1)
			{
				perror("shm_unlink");
				goto error;
			}
			//-------------------------------------------------------------
			//Printf the final result:
			timersub(&final_timestamp, &shm_pointer->start_timestamp, &delta_time);
			printf("Elapsed time: %5.6f s\n", (double) delta_time.tv_sec + delta_time.tv_usec/1000000.0);
			

			//-------------------------------------------------------------
		}
		else //Error
		{
			perror("fork");
			goto error;
		}
	}
	
	return 0;

error:
	exit(EXIT_FAILURE);
}
