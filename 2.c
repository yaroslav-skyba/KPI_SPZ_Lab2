#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	char buffer[1024];
	getcwd(buffer, sizeof(buffer));
	
	int logFile = creat("log.txt", 0644);

	if (logFile < 0) {
		printf("An error occurred while creating a file\n");
		return -1;
	}
	
	if (write(logFile, "Start\n", strlen("Start\n")) == -1) {
		printf("An error occurred while writing a file\n");
		return -1;
	}

	pid_t forkedPid = fork();

	if (forkedPid == -1) {
		printf("An error occurred while forking\n");
		return -1;
	} else if (forkedPid == 0) {
		if (setsid() == -1) {
			printf("An error occurred while setsidding\n");
			return -1;
		}
		
		if (chdir("/") == -1) {
			printf("An error occurred while changing a directory\n");
			return -1;
		}

		close(logFile);

		int openedFile = open("/dev/null", O_WRONLY);
		dup2(openedFile, STDIN_FILENO);
		dup2(openedFile, STDOUT_FILENO);
		dup2(openedFile, STDERR_FILENO);

		strcat(buffer, "/log.txt");

		logFile = open(buffer, O_WRONLY | O_APPEND, 0644);
		
		if (logFile == -1) {
			printf("An error occurred while opening a file\n");
			return -1;
		}

		snprintf(buffer, 1024, "uid - %d; forkedPid - %d; gid - %d; sid - %d;\n", getuid(), getpid(), getgid(), getsid(0));

		if (write(logFile, buffer, strlen(buffer)) == -1) {
			printf("An error occurred while writing a log file\n");
			return -1;
		}

		while (1) {
			sleep(1);
		}
	} else {
		if (write(logFile, "A parent: a child was created\n", strlen("A parent: a child was created\n")) == -1) {
			printf("An error occurred while writing a log file\n");
			return -1;
		}
		
		return 0;
	}
}
