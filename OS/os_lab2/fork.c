#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("[%d] This is first process\n", getpid());

	pid_t p = fork();
	if(p < 0) {
		printf("[%d] fork() failed!\n", getpid());
	} else if(p) {
		printf("[%d] fork() success!\n", getpid());
	}
	printf("[%d] fork() returned %d, I'm %s\n", getpid(), p, p == 0 ? "child" : "parent");
	printf("[%d] getpid() returned %d\n", getpid(), getpid());
	printf("[%d] getppid() returned %d\n", getpid(), getppid());
}
