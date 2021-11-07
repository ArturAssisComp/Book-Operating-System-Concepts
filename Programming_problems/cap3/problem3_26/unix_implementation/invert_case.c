#include <string.h>
#include <ctype.h>

void invert_case(char str[])
{
	size_t i;
	for(i = 0; i < strlen(str); i++)
	{
		if(isupper(str[i])) str[i] = tolower(str[i]);
		else if (islower(str[i])) str[i] = toupper(str[i]);
	}
}
