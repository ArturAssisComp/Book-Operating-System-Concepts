#include "bit_array.h"
#include "pid_table.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


pid_table *create_pid_table(int min_pid, int max_pid)
/**
 * Description: this function creates a pid table. The range of values allowed
 * is [min_pid, max_pid] (inclusive set). The pid table is returned if 
 * the operation was successful, and NULL is returned otherwise. 
 * After using this data structure, the user must call 'delete_pid_table'
 * passing table returned as argument by this function.
 *
 * Output: The function returns a pointer to the pid_table created. NULL is returned
 * if the function fails.
 *
 */
{
	size_t size_of_pid_table;
	pid_table *result = NULL;

	if(min_pid > max_pid) goto return_result;
	
	size_of_pid_table = max_pid - min_pid + 1;

	//Allocate memory for the pid table:
	result = malloc(sizeof *result);
	if(!result) goto return_result;

	//Initialize the new pid table:
	result->pid_bit_array = create_bit_array(size_of_pid_table);
	if(!(result->pid_bit_array)) 
	{
		free(result);
		result = NULL;
		goto return_result;
	}
	result->min_pid = min_pid;
	result->max_pid = max_pid;
	result->min_pid_available = min_pid;
	
	

return_result:
	return result;
}

void delete_pid_table(pid_table **target_pid_table_address)
/**
 * Description: This function deletes a pid table successfully and previously created using 
 * 'create_pid_table' and pointed by 'target_pid_table_address'. This function must be called
 * after using the pid table in order to free the memory previously allocated. The target
 * pid table will be set to NULL.
 */
{
	pid_table *target_pid_table;

	if(target_pid_table_address == NULL)
	{
		fprintf(stderr, "Error: target_pid_table_address is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if(*target_pid_table_address)
	{
		target_pid_table = *target_pid_table_address;
		if(target_pid_table->pid_bit_array) delete_bit_array(&(target_pid_table->pid_bit_array));
		free(target_pid_table);
		*target_pid_table_address = NULL;
	}
}

int check_pid(pid_table *target_pid_table, int pid)
/**
 * Description: This function checks if the pid 'pid' is available in 'target_pid_table'. 
 * If it is available, this function returns 1, if it is not available, this function 
 * returns 0, and if the 'pid' is out of range, returns -1.
 */
{
	size_t index;
	bit_value bit_value_related_to_pid;
	int result = -1;

	//Check if the target_pid_table is NULL:
	if(!target_pid_table)
	{
		fprintf(stderr, "check_pid: target_pid_table is NULL.\n");
		exit(EXIT_FAILURE);
	}
	//Check if the pid is out of range:
	if(pid < target_pid_table->min_pid || pid > target_pid_table->max_pid) goto return_result;

	//Check if the pid is available:
	index = pid - target_pid_table->min_pid;
	bit_value_related_to_pid = get_bit_value(target_pid_table->pid_bit_array, index);
	if(bit_value_related_to_pid == ZERO) result = 1; //pid available
	else if(bit_value_related_to_pid == ONE) result = 0;//pid is not available
	else
	{
		fprintf(stderr, "check_pid:pid table corrupted.\n");
		exit(EXIT_FAILURE);
	}

return_result:
	return result;

}

int free_pid(pid_table *target_pid_table, int pid)
/**
 * Description: This functions frees the pid 'pid' in the table 'target_pid_table'. 
 * The pid 'pid', if valid, becomes available after this operation.
 * If target_pid_table is NULL or pid is out of range, it returns -1.
 * If the operation is successful, this function returns 1.
 */
{
	size_t index;
	int current_pid_status;
	int result = -1;

	if(!target_pid_table) goto return_result;

	current_pid_status = check_pid(target_pid_table, pid);
	if(current_pid_status == 1) result = 1;
	else if (current_pid_status == 0)
	{
		index = pid - target_pid_table->min_pid;
		//Make the bit corresponding to 'pid' available:
		if(assign_value_to_bit(target_pid_table->pid_bit_array, index, ZERO) == -1)
		{
			fprintf(stderr, "free_pid:pid table corrupted.\n");
			exit(EXIT_FAILURE);
		}
		result = 1;

		//Update the min_pid_available if necessary:
		if (pid < target_pid_table->min_pid_available) target_pid_table->min_pid_available = pid;

	}
	

return_result:
	return result;


}

int alloc_min_pid_available(pid_table *target_pid_table, int *pid_allocated_address)
/**
 * Description: this function allocates the min pid available in the table
 * 'target_pid_table'. It returns 1 if successful, or it returns 0 if table 
 * is full, and finally, it returns -1 if an error happens (ex. 'target_pid_table'
 * is NULL).
 * If the new pid is successfully allocated, its value is saved into the variable
 * pointed by 'pid_allocated_address'.
 */
{
	size_t index, bit_array_size, num_of_set_bits;
	int current_pid;
	int result = -1;

	//Check if the input is NULL:
	if(!target_pid_table) goto return_result;

	//Check if the table is full:
	if(target_pid_table->min_pid_available > target_pid_table->max_pid)
	{
		result = 0;
		goto return_result;
	}

	//Allocate the pid requested:
	index = target_pid_table->min_pid_available - target_pid_table->min_pid;
	if(assign_value_to_bit(target_pid_table->pid_bit_array, index, ONE) == -1)
	{
		fprintf(stderr, "free_pid:pid table corrupted.\n");
		exit(EXIT_FAILURE);
	}
	result = 1;
	*pid_allocated_address = target_pid_table->min_pid_available;

	//Update the min_pid_available:
	bit_array_size = get_bit_array_size(target_pid_table->pid_bit_array);
	num_of_set_bits = get_num_of_set_bits(target_pid_table->pid_bit_array);
	if(bit_array_size == num_of_set_bits) target_pid_table->min_pid_available = target_pid_table->max_pid + 1;
	else if (num_of_set_bits < bit_array_size)
	{
		//Find the next min pid available:
		current_pid = target_pid_table->min_pid_available;
		while(check_pid(target_pid_table, ++current_pid) != 1);
		target_pid_table->min_pid_available = current_pid;
	}
	
	
	
return_result:
	return result;
}
