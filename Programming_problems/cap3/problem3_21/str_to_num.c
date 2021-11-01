#include "str_to_num.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

//types:
typedef enum
{
	START,
	PLUS_READ,
	CONSUME_INITIAL_BLANK,
	READING_DIGITS,
	CONSUME_FINAL_BLANK,
	DEFAULT
}automaton_state;

typedef enum
{
	BLANK,
	DIGIT,
	PLUS_SIGN,
	ELSE
}char_class;

typedef enum
{
	STR_TO_SZT
}automaton_name;

//Local function declarations:
static char_class get_char_class(char c);
static unsigned char digit_to_u_char(char digit);
static void update_state(automaton_state *current_state_address, void *pointer_to_input, automaton_name current_automaton);
static automaton_state update_state__STR_TO_SIZE_T(automaton_state current_state, char_class input);
static int accept_state(automaton_state current_state, automaton_name current_automaton);


//Function definition:
int str_to_size_t(char str[], size_t *storage_var_address)
/**
 * Description: This function converts the string 'str' that represents a number 
 * in base 10 to size_t and stores the result into 'storage_var_address'. If the 
 * operation is successful, it returns 1. It returns 0 otherwise.
 *
 * Fail conditions:
 * 	Returns 0:
 * 		Any of ->
 * 		- 'str' is not of the form [<blank>]*[+ | <empty>][0...9]+[<blank>]*
 * 		- 'storage_var_address' NULL;
 * 		- 'str' is NULL;
 *
 */
{
	int return_status = 0;
	size_t i, result = 0;
	char current_char;
	char_class input;
	automaton_state current_state = START;
	automaton_name current_automaton = STR_TO_SZT;

	if(!str || !storage_var_address) goto return_result;

	for(i = 0; i < strlen(str); i++)
	{
		current_char = str[i];
		switch(input = get_char_class(current_char))
		{
			case DIGIT:
				result = (result * 10) + (size_t) digit_to_u_char(current_char);
			case BLANK:
			case PLUS_SIGN:
			case ELSE:
				break;
			default://Error
				fprintf(stderr, "str_to_size_t: input not defined.");
				exit(EXIT_FAILURE);
				break;
		}

		//Update the state of the automaton:
		update_state(&current_state, (void *) (&input), current_automaton);

		if(current_state == DEFAULT) break;
	}

	//Check if the string was accepted:
	if(accept_state(current_state, current_automaton)) 
	{
		*storage_var_address = result;
		return_status = 1;
	}

return_result:
	return return_status;
}

static char_class get_char_class(char c)
/**
 * Description: This function returns the class of the char c. The class may be
 * BLANK, DIGIT, PLUS_SIGN, ELSE.
 */
{

	//Check if the char is '+':
	if(c == '+')        return PLUS_SIGN;
	else if(isblank(c)) return BLANK;
	else if(isdigit(c)) return DIGIT;
	else                return ELSE;
}

static unsigned char digit_to_u_char(char digit)
{
	return digit - '0';
}
static void update_state(automaton_state *current_state_address, void *pointer_to_input, automaton_name current_automaton)
{
	if(!current_state_address || !pointer_to_input) 
	{
		fprintf(stderr, "update_state: NULL input not accepted.\n");
		exit(EXIT_FAILURE);
	}

	switch(current_automaton)
	{
		case STR_TO_SZT:
			*current_state_address = update_state__STR_TO_SIZE_T(*current_state_address, *((char_class *)pointer_to_input));
			break;
		default:
			fprintf(stderr, "update_state: automaton not specified.\n");
			exit(EXIT_FAILURE);
			break;

	}
}

static automaton_state update_state__STR_TO_SIZE_T(automaton_state current_state, char_class input)
/**
 * Description: This function implements the rules for the automaton STR_TO_SIZE_T. It receives
 * the current state and the input and returns the next state.
 *
 *  Finite automaton STR_TO_SZT:
 *                                                             INPUTS
 *                              |--------------------------------------------------------------|
 *                              |         <blank>       |    '+'    |     [0..9]     |   else  |
 * |----------------------------|--------------------------------------------------------------|
 * |    (CURRENT  STATE)        |                         (NEXT STATE)                         |
 * |----------------------------|--------------------------------------------------------------|
 * |   START                    | CONSUME_INITIAL_BLANK | PLUS_READ | READING_DIGITS | DEFAULT |
 * |----------------------------|--------------------------------------------------------------|
 * |   PLUS_READ                |        DEFAULT        |  DEFAULT  | READING_DIGITS | DEFAULT |
 * |----------------------------|--------------------------------------------------------------|
 * |   CONSUME_INITIAL_BLANK    |           -           | PLUS_READ | READING_DIGITS | DEFAULT |
 * |----------------------------|--------------------------------------------------------------|
 * |   READING_DIGITS        (*)|        DEFAULT        |  DEFAULT  |        -       | DEFAULT |
 * |----------------------------|--------------------------------------------------------------|
 * |   CONSUME_FINAL_BLANK   (*)|           -           |  DEFAULT  |    DEFAULT     | DEFAULT |
 * |----------------------------|--------------------------------------------------------------|
 * |   DEFAULT                  |           -           |     -     |        -       |    -    |
 * |----------------------------|--------------------------------------------------------------|
 *
 *  OBS1: Accepting states are marked with '(*)'
 *  OBS2: '-' means that the next state is equal to the current state.
 */
{
	automaton_state next_state = current_state;

	switch(current_state)
	{
		case START:
			switch(input)
			{
				case BLANK:
					next_state = CONSUME_INITIAL_BLANK;
					break;
				case DIGIT:
					next_state = READING_DIGITS;
					break;
				case PLUS_SIGN:
					next_state = PLUS_READ;
					break;
				case ELSE:
					next_state = DEFAULT;
					break;
				default:
					fprintf(stderr, "update_state__STR_TO_SIZE_T: input not recognized.\n");
					goto error;
					break;
			}
			break;
		case PLUS_READ:
			switch(input)
			{
				case BLANK:
					next_state = DEFAULT;
					break;
				case DIGIT:
					next_state = READING_DIGITS;
					break;
				case PLUS_SIGN:
					next_state = DEFAULT;
					break;
				case ELSE:
					next_state = DEFAULT;
					break;
				default:
					fprintf(stderr, "update_state__STR_TO_SIZE_T: input not recognized.\n");
					goto error;
					break;
			}
			break;
		case CONSUME_INITIAL_BLANK:
			switch(input)
			{
				case BLANK:
					break;
				case DIGIT:
					next_state = READING_DIGITS;
					break;
				case PLUS_SIGN:
					next_state = PLUS_READ;
					break;
				case ELSE:
					next_state = DEFAULT;
					break;
				default:
					fprintf(stderr, "update_state__STR_TO_SIZE_T: input not recognized.\n");
					goto error;
					break;
			}
			break;
		case READING_DIGITS:
			switch(input)
			{
				case BLANK:
					next_state = DEFAULT;
					break;
				case DIGIT:
					break;
				case PLUS_SIGN:
					next_state = DEFAULT;
					break;
				case ELSE:
					next_state = DEFAULT;
					break;
				default:
					fprintf(stderr, "update_state__STR_TO_SIZE_T: input not recognized.\n");
					goto error;
					break;
			}
			break;
		case CONSUME_FINAL_BLANK:
			switch(input)
			{
				case BLANK:
					break;
				case DIGIT:
					next_state = DEFAULT;
					break;
				case PLUS_SIGN:
					next_state = DEFAULT;
					break;
				case ELSE:
					next_state = DEFAULT;
					break;
				default:
					fprintf(stderr, "update_state__STR_TO_SIZE_T: input not recognized.\n");
					goto error;
					break;
			}
			break;
		case DEFAULT:
			switch(input)
			{
				case BLANK:
				case DIGIT:
				case PLUS_SIGN:
				case ELSE:
					break;
				default:
					fprintf(stderr, "update_state__STR_TO_SIZE_T: input not recognized.\n");
					goto error;
					break;
			}
			break;
		default://Error
			fprintf(stderr, "update_state__STR_TO_SIZE_T: current state not recognized.\n");
			goto error;
			break;
	}

	return next_state;

error:
	exit(EXIT_FAILURE);
}

static int accept_state(automaton_state current_state, automaton_name current_automaton)
/**
 * Description: Checks if the automaton 'current_automaton' accepts the state 'current_state'.
 */
{
	int accept_current_state = 0;
	switch(current_automaton)
	{
		case STR_TO_SZT:
			if(current_state == READING_DIGITS || current_state == CONSUME_FINAL_BLANK) accept_current_state = 1;
			break;
		default:
			fprintf(stderr, "update_state: automaton not specified.\n");
			exit(EXIT_FAILURE);
			break;

	}

	return accept_current_state;
}
