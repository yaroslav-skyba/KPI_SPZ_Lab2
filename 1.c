#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print(const char *name) {
	printf("%s\n", name);
	printf("A process - %d\n", getpid());
	printf("A group - %d\n"(), getgid());
	printf("A user - %d\n"getuid());
	printf("A session - %d\n\n", getsid(0));
}

int main(int argc, char **argv) {
	print("A parent");
	
	pid_t forkedPid = fork();
	
	if (forkedPid < 0) {
		printf("An error occurred while forking\n");
		return -1;
	} else if (forkedPid == 0) {
		for (int i = 0; i < 3; i++) {
			print("A child");
		}
		
		printf("A child successfully finished an execution\n");
	} else {
		for (int i = 0; i < 3; i++) {
			print("A parent");
		}
		
		printf("Waiting for a child\n");
		wait(NULL);
		
		printf("A parent successfully finished an execution\n");
	}

	return 0;
}
