#include <stdio.h>
#include <stddef.h>

char* _strcpy(char* dest, const char *src)
{
	char* original_des = dest;

	while(*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}

	*dest = '\0';
	return original_des;
}


int main()
{

	char buffer[20];
	const char* source = "Hello, C!";

	char* copied_string = _strcpy(buffer, source);
	printf("Copied string: %s\n", copied_string);


	return 0;
}

