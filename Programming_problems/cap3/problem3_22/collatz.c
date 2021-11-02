#include "collatz.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LOCAL_BUF_SIZE 10000

//Function definitions:
void collatz_algorithm(char *buf, size_t n)
{
	char local_buf[LOCAL_BUF_SIZE];

	if(n == 0) 
	{
		fprintf(stderr, "Input may not be 0.\n");
		exit(EXIT_FAILURE);
	}
	
	sprintf(local_buf, "Collatz sequence for n = %zu\n", n);
	strcat(buf, local_buf);
	sprintf(local_buf, "[ %zu", n);
	strcat(buf, local_buf);
	while(n != 1)
	{
		n = (n % 2)?(3 * n + 1):(n / 2);
		sprintf(local_buf, " -> %zu", n);
		strcat(buf, local_buf);
	}
	sprintf(local_buf, "]\n");
	strcat(buf, local_buf);
}

