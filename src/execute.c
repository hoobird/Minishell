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

void	free_command_args(t_command_args **command_args)
{
	int i;

	i = 0;
	while (command_args[i])
	{
		free_tokenlist(&command_args[i]->tokenlist);
		free(command_args[i]);
		i++;
	}
	free(command_args);
}

void	path_list_free(char **path_list)
{
	int i;

	i = 0;
	while (path_list[i])
	{
		free(path_list[i]);
		i++;
	}
	free(path_list);
}

int	check_executable(char	**envpc, char **command_args)
{
	char	*paths;
	char	**path_list;
	int		i;
	char	*binary_path;

	paths = envpc_get_value(envpc, "PATH");
	if (paths == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command_args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (NOT_FOUND);
	}
	path_list = ft_split(paths, ':');
	i = 0;
	while (path_list[i])
	{
		paths = ft_strjoin(path_list[i], "/");
		binary_path = ft_strjoin(paths, command_args[0]);
		free(paths);
		if (check_file_type(binary_path) == 1 && check_file_permissions(binary_path, X_OK) == 1) // a file and can execute
		{
			path_list_free(path_list);
			*command_args = binary_path;
			return (EXECUTABLE);
		}
		free(binary_path);
		i++;
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command_args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	path_list_free(path_list);
	return (NOT_FOUND);
}

int	check_command_type(char	**envpc, char **command_args)
{
	printf("Checking command: %s\n", command_args[0]);
	if (ft_strcmp(command_args[0], "echo") == 0)
		return (BUILTIN_ECHO);
	else if (ft_strcmp(command_args[0], "cd") == 0)
		return (BUILTIN_CD);
	else if (ft_strcmp(command_args[0], "pwd") == 0)
		return (BUILTIN_PWD);
	else if (ft_strcmp(command_args[0], "export") == 0)
		return (BUILTIN_EXPORT);
	else if (ft_strcmp(command_args[0], "unset") == 0)
		return (BUILTIN_UNSET);
	else if (ft_strcmp(command_args[0], "env") == 0)
		return (BUILTIN_ENV);
	else if (ft_strcmp(command_args[0], "exit") == 0)
		return (BUILTIN_EXIT);
	return (check_executable(envpc, command_args));
}

int		command_args_len(t_command_args **command_args)
{
	int	i = 0;

	if (command_args)
	{
		while (command_args[i])
			i++;
	}
	return (i);
}

void	run_builtin(char ***envpc, char **command_args_string)
{
	int	outcome;

	outcome = 0;
	if (check_command_type(*envpc, command_args_string) == BUILTIN_ECHO)
		outcome = builtin_echo(command_args_string);
	else if (check_command_type(*envpc, command_args_string) == BUILTIN_CD)
		outcome = builtin_cd(&command_args_string[1], envpc);
	else if (check_command_type(*envpc, command_args_string) == BUILTIN_PWD)
		outcome = builtin_pwd(&command_args_string[1], envpc);
	else if (check_command_type(*envpc, command_args_string) == BUILTIN_EXPORT)
		outcome = builtin_export(&command_args_string[1], envpc);
	else if (check_command_type(*envpc, command_args_string) == BUILTIN_UNSET)
		outcome = builtin_unset(command_args_string, envpc);
	else if (check_command_type(*envpc, command_args_string) == BUILTIN_ENV)
		outcome = builtin_env(command_args_string, envpc);
	else if (check_command_type(*envpc, command_args_string) == BUILTIN_EXIT)
		builtin_exit_string(command_args_string);
	envpc_add(envpc, "?", ft_itoa(outcome));
}

void	run_in_child(t_command_args **command_args, int index, char ***envpc, char **command_args_string)
{
	int	i;

	if (fork() == 0)
	{
		if (command_args[index]->writefd != STDOUT_FILENO)
			dup2(command_args[index]->writefd, STDOUT_FILENO);
		if (command_args[index]->readfd != STDIN_FILENO)
			dup2(command_args[index]->readfd, STDIN_FILENO);
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
		// execute builtin
		run_builtin(envpc, command_args_string);
		builtin_exit(0);
	}
}

void	execute_in_child(t_command_args **command_args, int index, char ***envpc, char **command_args_string)
{
	int	i;

	if (fork() == 0)
	{
		if (command_args[index]->writefd != STDOUT_FILENO)
			dup2(command_args[index]->writefd, STDOUT_FILENO);
		if (command_args[index]->readfd != STDIN_FILENO)
			dup2(command_args[index]->readfd, STDIN_FILENO);
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
		execve(command_args_string[0], command_args_string, *envpc);
		printerror("minishell: command not found\n");
		builtin_exit(127);
	}
}

void	execution(t_command_args **command_args, char ***envpc)
{
	int i;
	char	**command_args_string;
	int		command_type;

	i = 0;
	while (command_args[i])
	{
		command_args_string = command_args_extraction(command_args[i]->tokenlist);
		command_type = check_command_type(*envpc, command_args_string);
		if (command_type  < 98)// builtin
		{
			// execute builtin
			if (command_args_len(command_args) == 1) // no pipes so run in parent
				run_builtin(envpc, command_args_string);
			else // pipes avail then run in child
				run_in_child(command_args, i, envpc, command_args_string);
		}
		else  if (command_type == EXECUTABLE)// executable
		{
			execute_in_child(command_args, i, envpc, command_args_string);
			free(command_args_string[0]);
		}
		// close all the pipes used
		if (command_args[i + 1] != NULL)
			close(command_args[i]->writefd);
		if (i != 0)
			close(command_args[i]->readfd);
		free(command_args_string);
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
