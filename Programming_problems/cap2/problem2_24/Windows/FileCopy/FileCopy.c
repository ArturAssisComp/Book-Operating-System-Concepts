/**
 * Author: Artur Assis Alves
 * Date: 14/06/2021
 * Title: File Copy - Windows Version
 *
 * This code was inspired by the following example provided by Microsoft:
 * https://docs.microsoft.com/en-us/windows/win32/seccrypto/example-c-program-encrypting-a-file
 */

 /**
  * Question 2.24 - Write a program that copies the contents of one file to a
  * destination file by prompting the user for the name of the source and destination
  * files. Write this program using either the POSIX or Windows API. Be sure to
  * include all necessary error checking, including ensuring that the source file exists.
  */


  //Libs:
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

//Define:
#define TRANSFER_BLOCK_SIZE 1000

//Declarations of the functions:
bool CopyFromTo(
	LPTSTR SourceFile,
	LPTSTR DestinationFile);

void MyHandleError(
	LPTSTR ErrorMessage,
	int ErrorNumber);

/**
 * Documentation -->
 * Description:
 * Inputs:
 * Output:
 */
int _tmain(int argc, TCHAR* argv[])
{
	//Check the arguments:
	if (argc != 3)
	{
		_tprintf(TEXT("Usage: Filecopy.exe <source file> <destination file>\n"));
		_tprintf(TEXT("Press any key to exit."));
		_gettch();
		return 1;
	}

	LPTSTR SourceFile = argv[1];
	LPTSTR DestinationFile = argv[2];

	// Call CopyFromTo:
	if (CopyFromTo(SourceFile, DestinationFile))
	{
		_tprintf(
			TEXT("Content from the file %s was successfully copied to the file %s. \n"),
			SourceFile,
			DestinationFile);
	}
	else
	{
		MyHandleError(
			TEXT("Error copying content from the source file to the destination file.\n"),
			GetLastError());
	}
	return 0;
}



/**
 * Function name: CopyFromTo
 * Description: this function copies the content from the source file described
 * by the filename 'SourceFile' to the destination file described by filename 
 * 'DestinationFile'. If the destination file does not exist, it is created. If 
 * the source file does not exist, no content is copied and no destination file 
 * is created.
 * 
 * Input: (LPTSTR) SourceFile --> The name of the source file. May be a relative 
 *                 name or an absolute name. If it is a relative name, the system 
 *  			   will search for that file in the current directory.
 *        (LPTSTR) DestinationFile --> The name of the destination file. May be a relative 
 *                 name or an absolute name. If it is a relative name, the system 
 *   			   will search for that file in the current directory.
 *
 * 
 * Output: (bool) --> true if the content from file 'SourceFile' was successfully copied to file 'DestinationFile'.
 * 
 */
bool CopyFromTo(
	LPTSTR SourceFileStr,
	LPTSTR DestinationFileStr)
{
	//---------------------------------------------------------------
	// Declare and initialize local variables.
	bool ReturnValue = false;
	HANDLE SourceFile = INVALID_HANDLE_VALUE;
	HANDLE DestinationFile = INVALID_HANDLE_VALUE;

	PBYTE Buffer = NULL;
	DWORD BytesProcessed;
	  

	//---------------------------------------------------------------
	// Open the source file. 
	SourceFile = CreateFile(
		SourceFileStr,
		FILE_READ_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE != SourceFile)
	{
		_tprintf(
			TEXT("The source plaintext file, %s, is open. \n"),
			SourceFileStr);
	}
	else
	{
		MyHandleError(
			TEXT("Error opening source plaintext file!\n"),
			GetLastError());
		goto Exit_CopyFromTo;
	}

	//---------------------------------------------------------------
	//Open the destination file:
	DestinationFile = CreateFile(
		DestinationFileStr,
		FILE_WRITE_DATA,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE != DestinationFile)
	{
		_tprintf(
			TEXT("The destination plaintext file, %s, is open/created. \n"),
			DestinationFileStr);
	}
	else
	{
		MyHandleError(
			TEXT("Error opening destination plaintext file!\n"),
			GetLastError());
		goto Exit_CopyFromTo;
	}

	//---------------------------------------------------------------
	//Allocate memory for the buffer:
	if (Buffer = (BYTE*)malloc(TRANSFER_BLOCK_SIZE + 1))
	{
		_tprintf(
			TEXT("Memory has been allocated for the buffer. \n"));
	}
	else
	{
		MyHandleError(TEXT("Out of memory. \n"), E_OUTOFMEMORY);
		goto Exit_CopyFromTo;
	}

	//---------------------------------------------------------------
	//Copy the content from the input file to the output file:
	bool ReachedEOF = FALSE;
	do
	{
		//-----------------------------------------------------------
		// Read up to dwBlockLen CHARs from the source file. 
		if (!ReadFile(
			SourceFile,
			Buffer,
			TRANSFER_BLOCK_SIZE,
			&BytesProcessed,
			NULL))
		{
			MyHandleError(
				TEXT("Error reading plaintext!\n"),
				GetLastError());
			goto Exit_CopyFromTo;
		}

		if (BytesProcessed < TRANSFER_BLOCK_SIZE)
		{
			ReachedEOF = TRUE;
		}

		//-----------------------------------------------------------
		// Write the buffered data to the destination file. 
		if (!WriteFile(
			DestinationFile,
			Buffer,
			BytesProcessed,
			&BytesProcessed,
			NULL))
		{
			MyHandleError(
				TEXT("Error writing ciphertext.\n"),
				GetLastError());
			goto Exit_CopyFromTo;
		}

		//-----------------------------------------------------------
		// End the do loop when the last block of the source file 
		// has been read and written to the destination 
		// file.
	} while (!ReachedEOF);


	//---------------------------------------------------------------
	//Successfully copied:
	ReturnValue = true;


	//---------------------------------------------------------------
Exit_CopyFromTo:
	// Close files.
	if (SourceFile)
	{
		CloseHandle(SourceFile);
	}

	if (DestinationFile)
	{
		CloseHandle(DestinationFile);
	}

	//---------------------------------------------------------------
	// Free memory. 
	if (Buffer)
	{
		free(Buffer);
	}
	//---------------------------------------------------------------

	return ReturnValue;
}


void MyHandleError(LPTSTR ErrorMessage, int ErrorNumber)
{
	_ftprintf(stderr, TEXT("An error occurred in the program. \n"));
	_ftprintf(stderr, TEXT("%s\n"), ErrorMessage);
	_ftprintf(stderr, TEXT("Error number %x.\n"), ErrorNumber);
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
