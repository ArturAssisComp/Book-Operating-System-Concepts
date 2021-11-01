#include "collatz.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


//Function definitions:
void collatz_algorithm(size_t n)
{
	if(n == 0) 
	{
		fprintf(stderr, "Input may not be 0.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Collatz sequence for n = %zu\n", n);
	printf("[ %zu", n);
	while(n != 1)
	{
		n = (n % 2)?(3 * n + 1):(n / 2);
		printf(" -> %zu", n);
	}
	printf("]\n");
}

