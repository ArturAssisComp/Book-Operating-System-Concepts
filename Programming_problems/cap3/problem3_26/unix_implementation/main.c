#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "invert_case.h"

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
	pid_t pid;
	char *current_word;
	char *buffer;
	const char blank_space = ' ';
	size_t i, j;
	size_t word_len;
	size_t buffer_size;
	int pipe_fd[2];
	int child_status;

	if(argc <= 1) 
	{
		fprintf(stderr, "Usage: reverse_case <string>\n");
		goto error;
	}

	//Create the pipe:
	if(pipe(pipe_fd) == -1)
	{
		perror("pipe");
		goto error;
	}

	//Calculate the size of the buffer:
	buffer_size = 0; //Increase 1 for the '\0' but decrease 1 for the ' ' added after the last word.
	for(i = 1; i < argc; i++)
	{
		current_word = argv[i];
		word_len = strlen(current_word);
		buffer_size += word_len + 1; //The lenght of the word plus the blank space.

	}

	//Allocate the buffer:
	buffer = calloc(buffer_size, sizeof *buffer);

	if(buffer == NULL)
	{
		perror("calloc");
		goto error;
	}
	

	//Fork:
	pid = fork();

	if(pid == 0) //Child process
	{


		//Read from the pipe and get the input string:
		int temp;
		if((temp = read(pipe_fd[READ_END], buffer, buffer_size - 1)) != buffer_size - 1)
		{

			fprintf(stderr, "Error(child): problem while reading from pipe.\n");
			goto error;
		}
		buffer[buffer_size - 1] = '\0'; 

		//Close the read end:
		close(pipe_fd[READ_END]);

		//Invert case:
		invert_case(buffer);

		//White to the pipe the result:
		if(write(pipe_fd[WRITE_END], buffer, buffer_size) != buffer_size)
		{
			fprintf(stderr, "Error(child): problem while writing to pipe.\n");
			goto error;
		}

		//Close the write end:
		close(pipe_fd[WRITE_END]);

	}
	else if (pid > 0) //Parent process
	{
		//Write the inputs from argv to the pipe:
		for(i = 1; i < argc; i++)
		{
			current_word = argv[i];
			word_len = strlen(current_word);
			//Write the i-th word:
			if(write(pipe_fd[WRITE_END], current_word, strlen(current_word)) != word_len)
			{
				fprintf(stderr, "Error(parent): problem while writing a word to pipe.\n");
				goto error;
			}

			//Separate this word from the next using a blank space:
			if(i != argc - 1) 
			{
				if(write(pipe_fd[WRITE_END], &blank_space, sizeof blank_space) != sizeof blank_space)
				{
					fprintf(stderr, "Error(parent): problem while writing a blank space to pipe.\n");
					goto error;
				}
			}

		}

		//Close the write end for the parent process:
		close(pipe_fd[WRITE_END]);

		//Wait for the child process to finish:
		if(wait(&child_status) == -1)
		{
			perror("wait");
			goto error;
		}

		if(child_status != 0) 
		{
			fprintf(stderr, "Problems while executing the child process.");
			goto error;
		}

		//Read the result from buffer and print it:
		if(read(pipe_fd[READ_END], buffer, buffer_size) != buffer_size)
		{
			fprintf(stderr, "Error: problem while reading from pipe.\n");
			goto error;
		}

		printf("Result: %s\n", buffer);

		//Close the read end:
		close(pipe_fd[READ_END]);

	}
	else //Error
	{
		perror("fork");
		goto error;
	}

	free(buffer);
	return 0;

error:
	exit(EXIT_FAILURE);
}
