#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	int fd[2];
	pid_t pid;
	char *execargv[3]; // Corrected the array size and arguments

	execargv[0] = "ls"; // The first argument should be the command itself
	execargv[1] = "-l";
	execargv[2] = NULL; // The argument list must be NULL-terminated
	execve("/bin/ls", execargv, envp);
	printf("IM STILL HERE");
	return (0);
}