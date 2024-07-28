#include "minishell.h"

int		count_commands_args(t_token *tokens)
{
	int		count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == COMMAND || tokens->type == ARGS)
			count++;
		tokens = tokens->next;
	}
	return (count);
}
char	**command_args_extraction(t_token *tokens)
{
	char	**output;
	int		i;

	i = 0;
	output = ft_calloc(sizeof(char *), count_commands_args(tokens) + 1);
	while (tokens)
	{
		if (tokens->type == COMMAND || tokens->type == ARGS)
		{
			output[i] = tokens->string;
			i++;
		}
		tokens = tokens->next;
	}
	return (output);
}

void	execution(t_command_args **command_args, char **envpc)
{
	int i;
	char	**command_args_string;

	i = 0;
	while (command_args[i])
	{
		printf("Executing %d\n", i);
		if (command_args[i]->cancelexec == 1)
		{
			printf("Skipping execution\n");
			i++;
			continue ;
		}
		command_args_string = command_args_extraction(command_args[i]->tokenlist);
		if (fork() == 0) // child process
		{
			if (command_args[i]->writefd != STDOUT_FILENO)
				dup2(command_args[i]->writefd, STDOUT_FILENO);
			if (command_args[i]->readfd != STDIN_FILENO)
				dup2(command_args[i ]->readfd, STDIN_FILENO);
			// close all pipes
			i = 0;
			while (command_args[i])
			{
				if (command_args[i + 1] != NULL)
					close(command_args[i]->writefd);
				if (i != 0)
					close(command_args[i]->readfd);
				i++;
			}
			// execute command
			execve(ft_strjoin("/bin/", command_args_string[0]), command_args_string, envpc);
			printerror("minishell: command not found\n");
			exit(127);
		}
		// parent process
		if (command_args[i + 1] != NULL)
			close(command_args[i]->writefd);
		if (i != 0)
			close(command_args[i]->readfd);	
		i++;
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
}


// // Test case
// char	**commands[4] = {(char *[]){"cat", NULL}, (char *[]){"cat", NULL},
// 		(char *[]){"ls", NULL}, NULL};

// int	execute(char ***cmds)
// {
// 	int	cmdindex;
// 	int	**pipelist;
// 	int	i;

// 	pipelist = generate_pipes_upgrade_struct(2);
// 	cmdindex = 0;
// 	while (cmds[cmdindex])
// 	{
// 		if (fork() == 0) // child process
// 		{
// 			printf("Child process %d\n", cmdindex);
// 			// prep for piping
// 			if (cmdindex == 0)
// 			{
// 				dup2(pipelist[cmdindex][1], 1); // redirect stdout to pipe write
// 			}
// 			else if (cmds[cmdindex + 1] == NULL)
// 			{
// 				dup2(pipelist[cmdindex - 1][0], 0);
// 					// redirect stdin to pipe read
// 			}
// 			else
// 			{
// 				dup2(pipelist[cmdindex - 1][0], 0);
// 					// redirect stdin to previous pipe read
// 				dup2(pipelist[cmdindex][1], 1);    
// 					// redirect stdout to current pipe write
// 			}
// 			// close all pipes
// 			i = 0;
// 			while (pipelist[i])
// 			{
// 				close(pipelist[i][0]);
// 				close(pipelist[i][1]);
// 				i++;
// 			}
// 			printf("Executing%d %s\n", cmdindex, cmds[cmdindex][0]);
// 			execve(ft_strjoin("/bin/", cmds[cmdindex][0]), cmds[cmdindex],
// 				NULL);
// 			exit(127);
// 		}
// 		// parent process
// 		printf("Parent process %d\n", cmdindex);
// 		// close pipes
// 		if (cmdindex == 0)
// 			close(pipelist[cmdindex][1]); // close write end of pipe
// 		else if (cmds[cmdindex + 1] == NULL)
// 			close(pipelist[cmdindex - 1][0]); // close read end of pipe
// 		else
// 		{
// 			close(pipelist[cmdindex - 1][0]); // close read end of previous pipe
// 			close(pipelist[cmdindex][1]);     // close write end of current pipe
// 		}
// 		cmdindex++;
// 	}
// 	while (waitpid(-1, NULL, 0) > 0)
// 		;
// 	return (0);
// }

// // cc execute.c piping.c ../Libft/libft.a
// int	main(void)
// {
// 	int **pipelist;
// 	pipelist = generate_pipes_upgrade_struct(2);
// 	printpipelist(pipelist);
// 	execute(commands);
// 	return (0);
// }