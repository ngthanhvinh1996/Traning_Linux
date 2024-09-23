#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define SIMPLE 0U

void signal_handler(int signal)
{
	printf("Signal number: %d\n", signal);
	if(SIGINT == signal)
	{
		printf("Received SIGINT\n");
	}

	exit(0);
}

int main(void)
{
#ifdef SIMPLE
	signal(SIGINT, signal_handler);
#else
	struct sigaction sa;

	memset(&sa, 0x00U, sizeof(sa));

	sa.sa_handler = signal_handler;

	if(-1 == sigaction(SIGTERM, &sa, NULL))
	{
		printf("Call sigaction error\n");
		return -1;
	}

#endif
	while(1)
	{

	}

	return 0;
}

