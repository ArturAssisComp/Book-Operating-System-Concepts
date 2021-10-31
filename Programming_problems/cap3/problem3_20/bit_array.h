#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H
#include <stdint.h>
#include <stddef.h>

//Macros:

/**
 * BIT_GROUP_SIZE defines the number of bits from a bit group that will be used by
 * the data structure bit_array. It must be at most sizeof (bit_group).
 */
#define BIT_GROUP_SIZE 32 


//Types:
typedef enum
{
	ZERO,
	ONE,
	INVALID
}bit_value;

typedef uint_fast32_t bit_group;

typedef struct
{
	bit_group *array_of_bit_groups;
	size_t num_of_bit_groups;
	size_t size;            //Number of bits in the array
	size_t num_of_set_bits; //Number of bits with value ONE
}bit_array;

//Function declarations:
bit_array *create_bit_array(size_t size);
void delete_bit_array(bit_array **target_bit_array_address);
bit_value get_bit_value(bit_array *target_bit_array, size_t index);
int assign_value_to_bit(bit_array *target_bit_array, size_t index, bit_value value);
size_t get_bit_array_size(bit_array *target_bit_array);
size_t get_num_of_set_bits(bit_array *target_bit_array);
#endif 
