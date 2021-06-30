/**
 * Author: Artur Assis Alves
 * Date: 24/07/2021
 * Title: FileCopy
 */

/**
 * Question 2.24 - Write a program that copies the contents of one file to a 
 * destination file by prompting the user for the name of the source and destination 
 * files. Write this program using either the POSIX or Windows API. Be sure to 
 * include all necessary error checking, including ensuring that the source file exists.
 */

//Libs:
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

//Macros:
#define NON_UNIX_STD_ERROR_NO -1
#define MAX_BUFFER_SZ 300

//Declarations of the functions:
bool copy_from_to(char source_file_name[], char destination_file_name[]);
void handle_error(char function_name[], char err_msg[], int err_no);

//Main:
/**
 * Documentation --> 
 * Description: 
 * Inputs:
 * Output:
 */
int main(int argc, char *argv[])
{
	//---------------------------------------------------------------
	//Variables:
	char function_name[] = "main";

	//---------------------------------------------------------------
	//Check the arguments:
	if (argc != 3)
	{
		printf("Usage: Filecopy.exe <source file> <destination file>\n");
		printf("Press ENTER to continue.");
		getchar();
		return 1;
	}
	//---------------------------------------------------------------
	char *source_file_name = argv[1];
	char *destination_file_name = argv[2];

	//Call copy_from_to:
	if (copy_from_to(source_file_name, destination_file_name))
	{
		printf("Content from the file %s was successfully copied to the file %s. \n",
			source_file_name,
			destination_file_name);
	}
	else
	{
		handle_error(
			function_name,
			"Error copying content from the source file to the destination file.",
			NON_UNIX_STD_ERROR_NO);
	}
	return 0;
	//---------------------------------------------------------------
	
}

//Definitions of the functions:
bool copy_from_to(char source_file_name[], char destination_file_name[])
{
	//---------------------------------------------------------------
	//Declare Variables:
	bool return_value = false;
	////////Continue: should I use fopen or open??????????????????????????????????????????????
	//Yes, I should use open (POSIX function).
	FILE source_fd, destination_fd;
	
	
	//---------------------------------------------------------------
}

void handle_error(char function_name[], char err_msg[], int err_no)
/**
 * Description: This function handles the error that happened while executing the 
 * function with name 'function_name'. It prints a customized message and the 
 * standard UNIX error message if there is any. The 'err_msg' should not be finished 
 * with newline because this function automatically adds this final newline.
 * 	This function does not print messages that are arbitrarilly long. If the 
 * size of the buffer is too short, change the value of the constant MAX_BUFFER_SZ. 
 *
 * Input: (char []) function_name --> The name of the function from which this 
 *        function is called.
 *        (char []) err_msg --> The custom message that will be printed to stderr.
 *        (int) err_no --> The number of the standard UNIX error defined in errno.h. 
 *        If it is not defined, NON_UNIX_STD_ERROR_NO must be used.
 *
 * Output: (void)
 */
{
	char buffer[MAX_BUFFER_SZ];

	//---------------------------------------------------------------
	//Print the customized error message:
	fprintf(stderr, "----------ERROR----------\n");
	snprintf(buffer, MAX_BUFFER_SZ, "Inside function: %s", function_name); 
	fprintf(stderr, "%s\n", buffer);
	snprintf(buffer, MAX_BUFFER_SZ, "Custom message: %s", err_msg); 
	fprintf(stderr, "%s\n", buffer);

	//---------------------------------------------------------------
	//Print the standard UNIX error message if there is any:
	if(err_no != NON_UNIX_STD_ERROR_NO)
	{
		snprintf(buffer, MAX_BUFFER_SZ, "Unix standard error msg: %sn", strerror(err_no)); 
		fprintf(stderr, "%s\n", buffer);
	}
	//---------------------------------------------------------------
	fprintf(stderr, "-------------------------\n");
}
