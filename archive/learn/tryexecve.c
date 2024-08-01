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

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	// Fork a child process
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		// Child process
		close(fd[0]);              
        // Close the read end of the pipe in the child
		dup2(fd[1], STDOUT_FILENO);
        // Redirect stdout to the write end of the pipe
		close(fd[1]);               // Close the original write end of the pipe

		// Prepare the arguments for execve
		execargv[0] = "casdjklasdjlkasdjklasdat"; // The first argument should be the command itself
		// execargv[1] = "-l";
		execargv[1] = NULL; // The argument list must be NULL-terminated

		// Execute the command
        printf("ERROR CODE: %d", execve("/bin/ls", execargv, envp));
        // If execve returns, it must have failed
        perror("execve failed");
        exit(EXIT_FAILURE);
	}
	else
	{
		// Parent process
		char buffer[1024];
		ssize_t bytesRead;

		close(fd[1]); // Close the write end of the pipe in the parent

		// Read the output from the child process
		while ((bytesRead = read(fd[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytesRead] = '\0'; // Null-terminate the string
			printf("%s", buffer);
		}

		close(fd[0]); // Close the read end of the pipe
		wait(NULL);   // Wait for the child process to finish
        printf("PARENT STILL ALIVE");
	}

	return (0);
}