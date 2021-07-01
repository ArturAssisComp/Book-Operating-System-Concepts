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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Macros:
#define NON_UNIX_STD_ERROR_NO -1
#define MAX_BUFFER_SZ 350
#define INVALID_FD -1
#define READ_FAILED -1
#define WRITE_FAILED -1
#define CLOSE_ERROR -1

//Data types:
typedef unsigned char byte;

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
		printf("Content from the file '%s' was successfully copied to the file '%s'. \n",
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
/**
 * Function name: copy_from_to
 * Description: this function copies the content from the source file described
 * by the filename 'source_file_name' to the destination file described by filename 
 * 'destination_file_name'. If the destination file does not exist, it is created,
 * otherwise the destination file's content is overwritten by the new content. If 
 * the source file does not exist, no content is copied and no destination file 
 * is created.
 * 
 * Input: (char []) source_file_name --> The name of the source file. May be a relative 
 *                 name or an absolute name. If it is a relative name, the system 
 *  			   will search for that file in the current directory.
 *        (char []) destination_file_name --> The name of the destination file. May be a relative 
 *                 name or an absolute name. If it is a relative name, the system 
 *   			   will search for that file in the current directory.
 *
 * 
 * Output: (bool) --> true if the content from file 'source_file_name' was 
 * successfully copied to file 'destination_file_name'.
 * 
 */
{
	//---------------------------------------------------------------
	//Declare Variables:
	/*General*/
	char function_name[] = "copy_from_to";
	bool return_value = false;
	
	/*Files*/
	int src_fd = INVALID_FD, dest_fd = INVALID_FD;

	/*Transfer information*/
	ssize_t bytes_read, bytes_written;
	size_t block_transfer_sz = 5 * MAX_BUFFER_SZ;
	byte buffer[block_transfer_sz];
	bool reached_EOF = false;
	
	
	//---------------------------------------------------------------
	//Open Source file:
	src_fd = open(source_file_name, O_RDONLY);

	if(src_fd == INVALID_FD)
	{
		handle_error(
			function_name,
			"Error while opening the source file.",
			errno);
		goto EXIT_COPY_FROM_TO;
	}
	else
	{
		printf("Source file successfully opened.\n");
	}
	
	//---------------------------------------------------------------
	//Create the Destination file:
	dest_fd = creat(destination_file_name, S_IRWXU | S_IRWXG);

	if(dest_fd == INVALID_FD)
	{
		handle_error(
			function_name,
			"Error while creating the destination file.",
			errno);
		goto EXIT_COPY_FROM_TO;
	}
	else
	{
		printf("Destination file successfully created.\n");
	}
	
	//---------------------------------------------------------------
	//Copy the content from source file to destination file using the
	//buffer to transfer each block of memory:
	
	do{
		//---------------------------------------------------------------
		//Read the next block of memory from source file:
		bytes_read = read(src_fd, buffer, block_transfer_sz);

		if(bytes_read == 0)
		{
			reached_EOF = true;
		}
		else if (bytes_read == READ_FAILED)
		{
			handle_error(
				function_name,
				"Error while reading a block of memory from the source file.",
				errno);
			goto EXIT_COPY_FROM_TO;
		}

		//---------------------------------------------------------------
		//Write the block of memory read from source file to destination
		//file:
		bytes_written = write(dest_fd, buffer, bytes_read);

		if(bytes_written == WRITE_FAILED)
		{
			handle_error(
				function_name,
				"Error while writing a block of memory to the destination file.",
				errno);
			goto EXIT_COPY_FROM_TO;
		}
		else if (bytes_written != bytes_read)
		{
			handle_error(
				function_name,
				"Information lost. Difference in size between blocks of memory read and written.",
				NON_UNIX_STD_ERROR_NO);
			goto EXIT_COPY_FROM_TO;
		}

		//---------------------------------------------------------------
	}while(!reached_EOF);
	

	//---------------------------------------------------------------
	//Content from source file was successfully copied to destination 
	//file:
	return_value = true;	

	//---------------------------------------------------------------
	//Exit setup:
EXIT_COPY_FROM_TO:
	//Close open files:
	if(dest_fd != INVALID_FD)
	{
		if(close(dest_fd) == CLOSE_ERROR)
		{
			handle_error(
				function_name,
				"Destination file was not properly closed.",
				errno);
		}
	}
		

	if(src_fd != INVALID_FD)
	{
		if(close(src_fd) == CLOSE_ERROR)
		{
			handle_error(
				function_name,
				"Source file was not properly closed.",
				errno);
		}
	}	
	
	//Return the final result:
	return return_value;

	//---------------------------------------------------------------
}

void handle_error(char function_name[], char err_msg[], int err_no)
/**
 * Function name: handle_error
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
