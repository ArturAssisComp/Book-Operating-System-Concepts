#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFF_SZ 4096

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
	pid_t pid;
	char *destination_file;
	char *target_file;
	char buffer[BUFF_SZ];
	int pipe_fd[2];
	int target_fd, destination_fd;
	int child_status;
	ssize_t bytes_read, bytes_written;

	//Check the input:
	if(argc != 3)
	{
		fprintf(stderr, "Usage: filecopy <file_to_be_copied> <destination_file>\n");
		goto error;
	}

	target_file      = argv[1];
	destination_file = argv[2];

	//Create the pipe:
	if(pipe(pipe_fd) == -1)
	{
		perror("pipe");
		goto error;
	}

	//Fork the current process:
	pid = fork();

	if(pid == 0) //Child
	{
		//Close write end of the pipe:
		close(pipe_fd[WRITE_END]);

		//Create the destination_file:
		if((destination_fd = open(destination_file, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU | S_IRGRP | S_IWGRP)) == -1) 
		{
			perror("open");
			goto error;
		}

		//Read from pipe and write to destination file:
		do
		{
			if((bytes_read = read(pipe_fd[READ_END], buffer, BUFF_SZ)) == -1)
			{
				perror("read");
				goto error;
			}
			if((bytes_written = write(destination_fd, buffer, bytes_read)) == -1)
			{
				perror("write");
				goto error;
			}

			if(bytes_read != bytes_written)
			{
				fprintf(stderr, "Error(child): number of bytes read should be equal to number of bytes written.\n");
				goto error;
			}
		}while(bytes_read != 0);


		//Close files:
		close(destination_fd);
		close(pipe_fd[READ_END]);

	}
	else if(pid > 0)//Parent
	{
		//Close read end of the pipe:
		close(pipe_fd[READ_END]);

		//Open the target_file:
		if((target_fd = open(target_file, O_RDONLY)) == -1) 
		{
			perror("open");
			goto error;
		}

		//Write the content from target_file to the pipe:
		do
		{
			if((bytes_read = read(target_fd, buffer, BUFF_SZ)) == -1)
			{
				perror("read");
				goto error;
			}
			if((bytes_written = write(pipe_fd[WRITE_END], buffer, bytes_read)) == -1)
			{
				perror("write");
				goto error;
			}

			if(bytes_read != bytes_written)
			{
				fprintf(stderr, "Error(parent): number of bytes read should be equal to number of bytes written.\n");
				goto error;
			}
		}while(bytes_read != 0);

		//Close files:
		close(target_fd);
		close(pipe_fd[WRITE_END]);

		//Wait for the child:
		if(wait(&child_status) == -1)
		{
			perror("wait");
			goto error;
		}
		if(child_status != 0)
		{
			fprintf(stderr, "Error(parent): Some error occurred during the execution of the child process.\n");
			goto error;
		}

	}
	else//Error
	{
		perror("fork");
		goto error;
	}
	return 0;

error:
	exit(EXIT_FAILURE);
}
