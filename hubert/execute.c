#include "minishell.h"

// Test case
char	**commands[4] = {(char *[]){"cat", NULL}, (char *[]){"cat", NULL},
		(char *[]){"ls", NULL}, NULL};

int	execute(char ***cmds)
{
	int	cmdindex;
	int	**pipelist;
	int	i;

	pipelist = generate_pipes_upgrade_struct(2);
	cmdindex = 0;
	while (cmds[cmdindex])
	{
		if (fork() == 0) // child process
		{
			printf("Child process %d\n", cmdindex);
			// prep for piping
			if (cmdindex == 0)
			{
				dup2(pipelist[cmdindex][1], 1); // redirect stdout to pipe write
			}
			else if (cmds[cmdindex + 1] == NULL)
			{
				dup2(pipelist[cmdindex - 1][0], 0);
					// redirect stdin to pipe read
			}
			else
			{
				dup2(pipelist[cmdindex - 1][0], 0);
					// redirect stdin to previous pipe read
				dup2(pipelist[cmdindex][1], 1);    
					// redirect stdout to current pipe write
			}
			// close all pipes
			i = 0;
			while (pipelist[i])
			{
				close(pipelist[i][0]);
				close(pipelist[i][1]);
				i++;
			}
			printf("Executing%d %s\n", cmdindex, cmds[cmdindex][0]);
			execve(ft_strjoin("/bin/", cmds[cmdindex][0]), cmds[cmdindex],
				NULL);
			exit(127);
		}
		// parent process
		printf("Parent process %d\n", cmdindex);
		// close pipes
		if (cmdindex == 0)
			close(pipelist[cmdindex][1]); // close write end of pipe
		else if (cmds[cmdindex + 1] == NULL)
			close(pipelist[cmdindex - 1][0]); // close read end of pipe
		else
		{
			close(pipelist[cmdindex - 1][0]); // close read end of previous pipe
			close(pipelist[cmdindex][1]);     // close write end of current pipe
		}
		cmdindex++;
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (0);
}

// cc execute.c piping.c ../Libft/libft.a
int	main(void)
{
	int **pipelist;
	pipelist = generate_pipes_upgrade_struct(2);
	printpipelist(pipelist);
	execute(commands);
	return (0);
}