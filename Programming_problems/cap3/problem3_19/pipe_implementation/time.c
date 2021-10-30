#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1


int main(int argc, char *argv[])
{
	int child_process_status;
	int pipe_fd[2];
	char *command;
	char *new_argv[] = {NULL, NULL};
	char *new_env[] = {NULL};
	pid_t pid;
	struct timeval  start_timestamp, final_timestamp, delta_time;
	double seconds_elapsed;
	ssize_t bytes_read, bytes_written;

	if(argc != 2)
	{
		fprintf(stderr, "Correct use: %s <command>\n", argv[0]);
		goto error;
	}

	command = argv[1];
	//-------------------------------------------------------------
	//Create pipe:
	if(pipe(pipe_fd) == -1)
	{
		perror("pipe");
		goto error;
	}
	//-------------------------------------------------------------

	pid = fork();
	if(pid == 0)//Child
	{
		//-------------------------------------------------------------
		//Close the unused end of the pipe:
		close(pipe_fd[READ_END]);

		//-------------------------------------------------------------
		//Get the start timestamp:
		if(gettimeofday(&start_timestamp, NULL) == -1)
		{
			perror("gettimeofday");
			goto error;
		}


		//-------------------------------------------------------------
		//Write the value of the pointer for the start_timestamp from the pipe:
		ssize_t total_bytes_written = 0;
		while(1)
		{
			bytes_written = write(pipe_fd[WRITE_END], (unsigned char *) &start_timestamp + total_bytes_written, sizeof start_timestamp - total_bytes_written);
			if(bytes_written > 0) total_bytes_written += bytes_written;
			else break;
		}
		
		if(bytes_written == -1)
		{
			perror("write");
			goto error;
		}

		if(total_bytes_written != sizeof start_timestamp)
		{
			fprintf(stderr, "Message written into pipe is corrupted.\n");
			goto error;
		}

		close(pipe_fd[WRITE_END]);

		
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
		//Close the unused end of the pipe:
		close(pipe_fd[WRITE_END]);

		//-------------------------------------------------------------
		//Read the start_timestamp using the pipe:
		ssize_t total_bytes_read = 0;
		while(1)
		{

			bytes_read = read(pipe_fd[READ_END], (unsigned char *) &start_timestamp + total_bytes_read, sizeof start_timestamp - total_bytes_read);
			if(bytes_read > 0) total_bytes_read += bytes_read;
			else break;
		}

		if(bytes_read == -1)
		{
			perror("read");
			goto error;
		}

		if(total_bytes_read != sizeof start_timestamp)
		{
			fprintf(stderr, "Message read from pipe is corrupted.\n");
			goto error;
		}

		close(pipe_fd[READ_END]);

		//-------------------------------------------------------------
		//Wait for the child process to finish:
		if (wait(&child_process_status) == -1)
		{
			perror("wait");
			goto error;
		}

		if(child_process_status != 0)
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
		//Printf the final result:
		timersub(&final_timestamp, &start_timestamp, &delta_time);
		printf("Elapsed time: %5.6f s\n", (double) delta_time.tv_sec + delta_time.tv_usec/1000000.0);
		

		//-------------------------------------------------------------
	}
	else //Error
	{
		perror("fork");
		goto error;
	}
	
	return 0;

error:
	exit(EXIT_FAILURE);
}
