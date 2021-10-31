#include <stddef.h>
#include <stdlib.h>
#include "bit_array.h"

#include <stdio.h>

//Function definitions:
bit_array *create_bit_array(size_t size)
/**
 * Description: This function creates and initializes a bit_array data structure.
 * Its size is 'size' and each bit is initialized with value 0. The user must call
 * delete_bit_array after using the data structure.
 *
 * Output: The function returns a pointer to the bit_array created. NULL is returned
 * if the function fails.
 */
{
	size_t bit_group_size    = BIT_GROUP_SIZE;
	size_t num_of_bit_groups = size / bit_group_size + ((size % bit_group_size)?1:0);
	size_t offset, relative_bit_index;
	bit_array *new_bit_array = NULL;

	//--------------------------------------------------------------------------------
	//Allocate memory for bit array:
	new_bit_array = malloc(sizeof *new_bit_array);
	if(new_bit_array == NULL) goto return_value;

	//Initialize the bit_array:
	new_bit_array->array_of_bit_groups = calloc(num_of_bit_groups, sizeof *(new_bit_array->array_of_bit_groups));
	if(new_bit_array->array_of_bit_groups == NULL) 
	{
		free(new_bit_array);
		new_bit_array = NULL;
		goto return_value;
	}

	new_bit_array->num_of_bit_groups = num_of_bit_groups;
	new_bit_array->size = size;
	new_bit_array->num_of_set_bits = 0;
	
	//--------------------------------------------------------------------------------
	//Return value:
return_value:
	return new_bit_array;
	//--------------------------------------------------------------------------------

}


void delete_bit_array(bit_array **target_bit_array_address)
/**
 * Description: This function deletes a previously created (using create_bit_array) 
 * bit array pointed by 'target_bit_array_address'. All allocated memory will be 
 * freed and the target_bit_array will be pointed to NULL.
 *
 * Input: (bit_array **) target_bit_array_address --> Address of the bit array previously 
 *                                                    created using the function
 *                                                    'create_bit_array'.
 */
{
	bit_array *target_array;

	if(target_bit_array_address == NULL)
	{
		fprintf(stderr, "Error: target_bit_array_address is NULL.\n");
		exit(EXIT_FAILURE);
	}

	if(*target_bit_array_address)
	{
		target_array = *target_bit_array_address;
		if(target_array->array_of_bit_groups) free(target_array->array_of_bit_groups);
		free(target_array);
		*target_bit_array_address = NULL;
	}
}


bit_value get_bit_value(bit_array *target_bit_array, size_t index)
/**
 * Description: This function returns the value of a bit with index 'index'
 * from bit array 'bit_array'. If the index is out of range (greater than the 
 * bit array's size) or the 'target_bit_array' is NULL, the value returned is INVALID. 
 *
 * Input: (bit_array *) target_bit_array --> Bit array previously created using
 *                                           the function 'create_bit_array'.
 *
 * Output: (bit_value) -> The value of the bit from 'target_bit_array' with index
 *                        'index'. May be either ZERO or ONE if the bit is valid,
 *                        or INVALID if the bit is invalid.
 */
{
	size_t bit_group_size = BIT_GROUP_SIZE;
	size_t bit_group_index = index / bit_group_size;
	bit_group mask = 1;
	mask <<= index % bit_group_size;
	bit_value result = INVALID;

	if(target_bit_array == NULL || index >= target_bit_array->size || bit_group_index > target_bit_array->num_of_bit_groups) goto return_result;

	//Get the value from a valid bit:
	result = (target_bit_array->array_of_bit_groups[bit_group_index] & mask)?ONE:ZERO;


return_result:
	return result;

}


int assign_value_to_bit(bit_array *target_bit_array, size_t index, bit_value value)
/**
 * Description: This function assigns the value 'value' (may not be INVALID) to 
 * the bit with index 'index' from the bit array 'target_bit_array'. If the operation
 * is successful, it returns 0. It returns -1 otherwise.
 *
 * Conditions to fail: 
 * 	Any of ->
 * 	- 'index' out of range;
 * 	- 'value' equals INVALID;
 * 	- 'target_bit_array' equals NULL;
 */
{
	size_t bit_group_size = BIT_GROUP_SIZE;
	size_t bit_group_index = index / bit_group_size;
	bit_group mask = 1;
	bit_value old_value;
	
	mask <<= (index % bit_group_size);


	int result = -1;

	//Fail conditions:
	if(target_bit_array == NULL || value == INVALID || index >= target_bit_array->size) goto return_result;

	//Assign 'value' to the target bit:
	old_value = (target_bit_array->array_of_bit_groups[bit_group_index] & mask)?ONE:ZERO;

	if(old_value != value)
	{

		target_bit_array->array_of_bit_groups[bit_group_index] ^= mask;

		//Update the number of bits set:
		if(value == ONE) target_bit_array->num_of_set_bits++;
		else if (value == ZERO) target_bit_array->num_of_set_bits--;
	}
	result = 0;


return_result:
	return result;
}

size_t get_bit_array_size(bit_array *target_bit_array)
{
	return target_bit_array->size;
}

size_t get_num_of_set_bits(bit_array *target_bit_array)
{
	return target_bit_array->num_of_set_bits;
}
