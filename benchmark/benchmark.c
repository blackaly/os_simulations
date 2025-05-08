#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>


#define NUM_ITER 10000

int main()
{

	int pipe_1[2], pipe_2[2];
	pid_t pid;
	struct timeval start, end;

	if(pipe(pipe_1) == -1 || pipe(pipe_2) == -1)
	{
		perror("Error in piping");
		exit(1);
	}

	pid = fork();
	if(pid < 0)
	{
		perror("Error in forking");
		exit(1);
	}


	if(pid == 0)
	{
		char buf;
		char* buf_2 = "I have a girlfriend";
		for(int i = 0; i < NUM_ITER; ++i)
		{
			read(pipe_1[0], &buf, 1);
			write(pipe_2[1], &buf_2, 1);
		}
	}
	else
	{
		char buf;
		char* buf_2 = "I don't have a girlfriend because I am poor!";
		gettimeofday(&start, NULL);

		for(int i = 0; i < NUM_ITER; ++i)
		{
			write(pipe_1[1], &buf_2, 1);
			read(pipe_2[0], &buf, 1);
		}

		gettimeofday(&end, NULL);
		wait(NULL);

		long seconds = end.tv_sec - start.tv_sec;
		long useconds = end.tv_usec - start.tv_usec;
		long total_time = seconds * 1000000  + useconds;

		        printf("Average context switch time: %.2f microseconds\n", (double)total_time / (NUM_ITER * 2));
	}

	return 0;
}
