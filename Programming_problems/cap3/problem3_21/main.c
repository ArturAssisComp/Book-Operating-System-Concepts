#include "collatz.h"
#include "str_to_num.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	pid_t pid;
	size_t n;
	int child_status;

	//Check the input:
	if(argc != 2)
	{
		fprintf(stderr, "Usage: collatz <n>\n");
		exit(EXIT_FAILURE);
	}
	if(str_to_size_t(argv[1], &n) != 1 || n == 0)
	{
		fprintf(stderr, "Input must be a positive integer (n > 0).\n");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if(pid == 0)//Child
	{
		collatz_algorithm(n);
	}
	else if(pid > 0)//Parent
	{
		if(wait(&child_status) == -1)
		{
			perror("wait");
			exit(EXIT_FAILURE);
		}
		if(child_status != 0)
		{
			fprintf(stderr, "Error during child process execution.\n");
			exit(EXIT_FAILURE);
		}
	}
	else//Error
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}


	return 0;
}
