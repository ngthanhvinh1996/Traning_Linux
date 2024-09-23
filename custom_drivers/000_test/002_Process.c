#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *agrv[])
{
	pid_t pid;
	int status;
	int rv;

	pid = fork();
	
	if(0 > pid)
	{
		printf("fork() failed\n");
		return -1;
	}

	if(0 == pid)
	{
		printf("Child process\n");
		printf("Child my PID: %d, my parent PID: %d\n", getpid(), getppid());
		while(1)
		{
		
		}
	}
	else
	{
		printf("Parent process\n");
		printf("Parent my PID: %d", getpid());
		rv = wait(&status);
		if(-1 == rv)
		{
			printf("wait() failed\n");
		}

		printf("Status: %d\n", status);
	}
}
