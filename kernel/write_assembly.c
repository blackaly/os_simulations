#include <stdio.h>
#include <unistd.h>

size_t w_assm(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	__asm__ volatile (
		"syscall"
		: "=a" (ret)
		: "a" (1),
		  "D" (fd),
		  "S" (buf),
		  "d" (count)
		: "rcx", "r11", "memory"

	);

	return ret;
}


int main()
{

	const char* message = "Hello from direct system call\n";
	size_t message_len  = 0;

	while(message[message_len] != '\0')
	{
		message_len++;
	}

	printf("Calling my_write system call wrapper.. \n");
	ssize_t bytes_written = w_assm(1, message, message_len);

	if(bytes_written == -1) printf("Error writing to stdout\n");
	else printf("Successfully wrote %zd bytes using direct system call.\n", bytes_written);

	return 0;
}
