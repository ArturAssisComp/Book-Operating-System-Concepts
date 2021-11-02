#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "collatz.h"
#include "str_to_num.h"

#define SHM_SIZE 65536

int main(int argc, char *argv[])
{
	pid_t pid;
	size_t n;
	int child_status;
	int shm_fd;
	char shm_name[] = "/collatz_shm_memory";
	char *shm_buf;

	//Check the input:
	if(argc != 2)
	{
		fprintf(stderr, "Usage: collatz <n>\n");
		goto error;
	}
	if(str_to_size_t(argv[1], &n) != 1 || n == 0)
	{
		fprintf(stderr, "Input must be a positive integer (n > 0).\n");
		goto error;
	}


	//Create the shared memory region:
	shm_fd = shm_open(shm_name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if(shm_fd == -1)
	{
		perror("shm_open");
		goto error;
	}

	//Configure the size of the shared memory region:
	if(ftruncate(shm_fd, SHM_SIZE * sizeof *shm_buf) == -1)
	{
		perror("ftruncate");
		goto error;
	}

	//Map the shared memory region:
	shm_buf = mmap(NULL, sizeof *shm_buf, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(shm_buf == MAP_FAILED)
	{
		perror("mmap");
		goto error;
	}
	
	//Close shm_fd:
	if(close(shm_fd) == -1)
	{
		perror("close");
		goto error;
	}

	pid = fork();

	if(pid == 0)//Child
	{
		collatz_algorithm(shm_buf, n);
	}
	else if(pid > 0)//Parent
	{
		
		if(wait(&child_status) == -1)
		{
			perror("wait");
			goto error;
		}
		if(child_status != 0)
		{
			fprintf(stderr, "Error during child process execution.\n");
			goto error;
		}

		//Print the result:
		printf("%s", shm_buf);

		//Unlink the shared memory:
		if(shm_unlink(shm_name) == -1)
		{
			perror("shm_unlink");
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
