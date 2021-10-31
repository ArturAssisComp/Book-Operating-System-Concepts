#include "../bit_array.h"
#include <stdio.h>

//Local function declarations:
static void print_bit_array(bit_array *target_bit_array);
static void print_bit_array_with_details(bit_array *target_bit_array);

int main(void)
{
	//Create bit array of size 3:
	printf("Create a bit array of size 3\n");
	bit_array *my_bit_array = create_bit_array(3);
	print_bit_array_with_details(my_bit_array);
	printf("\n");
	printf("Get bit with index %zu -> bit value: %d\n", 0, get_bit_value(my_bit_array, 0));
	printf("Get bit with index %zu -> bit value: %d\n", 1, get_bit_value(my_bit_array, 1));
	printf("Get bit with index %zu -> bit value: %d\n", 2, get_bit_value(my_bit_array, 2));
	printf("Get bit with index %zu -> bit value: %d\n", 3, get_bit_value(my_bit_array, 3));
	printf("\n");
	printf("Assign some values to some bits:\n");
	printf("Assign value ONE to bit with index 0\n");
	if (assign_value_to_bit(my_bit_array, 0, ONE) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value ONE to bit with index 2\n");
	if (assign_value_to_bit(my_bit_array, 2, ONE) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value ZERO to bit with index 3\n");
	if (assign_value_to_bit(my_bit_array, 3, ZERO) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value INVALID to bit with index 0\n");
	if (assign_value_to_bit(my_bit_array, 0, INVALID) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value ZERO to bit with index 1\n");
	if (assign_value_to_bit(my_bit_array, 1, ZERO) == -1) printf("Failed\n");
	else printf("Success\n");
	print_bit_array_with_details(my_bit_array);
	printf("Get bit with index %zu -> bit value: %d\n", 0, get_bit_value(my_bit_array, 0));
	printf("Get bit with index %zu -> bit value: %d\n", 1, get_bit_value(my_bit_array, 1));
	printf("Get bit with index %zu -> bit value: %d\n", 2, get_bit_value(my_bit_array, 2));
	printf("Get bit with index %zu -> bit value: %d\n", 3, get_bit_value(my_bit_array, 3));
	printf("\n");

	//Create bit array of size 36:
	printf("Create a bit array of size 36\n");
	bit_array *my_bit_array1 = create_bit_array(36);
	print_bit_array_with_details(my_bit_array1);
	printf("\n");
	printf("Get bit with index %zu -> bit value: %d\n", 0, get_bit_value(my_bit_array1, 0));
	printf("Get bit with index %zu -> bit value: %d\n", 1, get_bit_value(my_bit_array1, 1));
	printf("Get bit with index %zu -> bit value: %d\n", 5, get_bit_value(my_bit_array1, 5));
	printf("Get bit with index %zu -> bit value: %d\n", 19, get_bit_value(my_bit_array1, 19));
	printf("Get bit with index %zu -> bit value: %d\n", 36, get_bit_value(my_bit_array1, 36));
	printf("\n");
	printf("Assign some values to some bits:\n");
	printf("Assign value ONE to bit with index 0\n");
	if (assign_value_to_bit(my_bit_array1, 0, ONE) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value ONE to bit with index 19\n");
	if (assign_value_to_bit(my_bit_array1, 19, ONE) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value ZERO to bit with index 3\n");
	if (assign_value_to_bit(my_bit_array1, 3, ZERO) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value INVALID to bit with index 0\n");
	if (assign_value_to_bit(my_bit_array1, 0, INVALID) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value ZERO to bit with index 1\n");
	if (assign_value_to_bit(my_bit_array1, 1, ZERO) == -1) printf("Failed\n");
	else printf("Success\n");
	printf("Assign value ONE to bit with index 1\n");
	if (assign_value_to_bit(my_bit_array1, 1, ONE) == -1) printf("Failed\n");
	else printf("Success\n");
	print_bit_array_with_details(my_bit_array1);
	printf("Get bit with index %zu -> bit value: %d\n", 0, get_bit_value(my_bit_array1, 0));
	printf("Get bit with index %zu -> bit value: %d\n", 1, get_bit_value(my_bit_array1, 1));
	printf("Get bit with index %zu -> bit value: %d\n", 5, get_bit_value(my_bit_array1, 5));
	printf("Get bit with index %zu -> bit value: %d\n", 19, get_bit_value(my_bit_array1, 19));
	printf("Get bit with index %zu -> bit value: %d\n", 36, get_bit_value(my_bit_array1, 36));

	//Delete the bit arrays created:
	delete_bit_array(&my_bit_array);
	delete_bit_array(&my_bit_array1);
}


//Local function definitions:
static void print_bit_array(bit_array *target_bit_array)
{
	size_t i, bit_group_size = BIT_GROUP_SIZE;
	bit_group mask;

	if(target_bit_array == NULL) printf("NULL");
	else
	{
		printf("[ ");
		if(target_bit_array->size > 0)
		{
			//Print the first element:
			mask = 1;
			printf("%d", (target_bit_array->array_of_bit_groups[0] & mask)?1:0);
			//Print the remaining elements:
			for(i = 1; i < target_bit_array->size; i++)
			{
				printf(", %d", (target_bit_array->array_of_bit_groups[i / bit_group_size] & (mask << (i % bit_group_size)))?1:0);
			}
		}
		printf("]");
	}
}

static void print_bit_array_with_details(bit_array *target_bit_array)
{
	printf("Bit Array -> \n");
	if(target_bit_array)
	{
		printf("\t-Array --> "); print_bit_array(target_bit_array); printf("\n");
		printf("\t-Size                 : %d\n", target_bit_array->size);
		printf("\t-Number of bits set   : %d\n", target_bit_array->num_of_set_bits);
		printf("\t-Number of bit groups : %d\n", target_bit_array->num_of_bit_groups);
	}
	else printf("\t-NULL\n");
}
