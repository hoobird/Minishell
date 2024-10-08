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

void	print_char_2d_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		ft_putstr_fd(array[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	ft_putstr_fd("\n", 1);
}

int	check_executable_in_path(char **envpc, char **command_args)
{
	char	*paths;
	char	**path_list;
	int		i;
	char	*binary_path;

	paths = envpc_get_value(envpc, "PATH");
	if (paths != NULL) // if PATH exists
	{
		path_list = ft_split(paths, ':');
		i = 0;
		while (path_list[i])
		{
			paths = ft_strjoin(path_list[i], "/");
			binary_path = ft_strjoin(paths, command_args[0]);
			free(paths);
			if (check_file_type(binary_path) == 1 && check_file_permissions(binary_path, X_OK) == 1)
			{
				// printf("binary_path = %s\n", binary_path);
				path_list_free(path_list);
				free(binary_path);
				return (EXECUTABLE_PATH);
			}
			free(binary_path);
			i++;
		}
		path_list_free(path_list);
	}
	return (NOT_FOUND);
}

char	*get_executable_in_path(char **envpc, char **command_args)
{
	char	*paths;
	char	**path_list;
	int		i;
	char	*binary_path;

	paths = envpc_get_value(envpc, "PATH");
	path_list = ft_split(paths, ':');
	i = 0;
	while (path_list[i])
	{
		paths = ft_strjoin(path_list[i], "/");
		binary_path = ft_strjoin(paths, command_args[0]);
		free(paths);
		if (check_file_type(binary_path) == 1 && check_file_permissions(binary_path, X_OK) == 1)
		{
			break;
		}
		free(binary_path);
		i++;
	}
	path_list_free(path_list);
	return (binary_path);
}

int	check_executable(char **envpc, char **command_args)
{
	if (ft_strchr(command_args[0], '/') == NULL) // if no path
	{
		if (check_executable_in_path(envpc, command_args) == EXECUTABLE_PATH) // check if in path
			return (EXECUTABLE_PATH);
		else
			return (NOT_FOUND);
	}
	if (check_file_permissions(command_args[0], F_OK) == 0) // file not found
		return (NO_SUCH_FILE_OR_DIRECTORY);
	if (check_file_type(command_args[0]) == 2) // directory
		return (DIRECTORY);
	if (check_file_type(command_args[0]) == 1) // file
	{
		if (check_file_permissions(command_args[0], X_OK) == 1) // executable
			return (EXECUTABLE);
		else // no permission
			return (PERMISSION_DENIED);
	}
	return (NOT_FOUND);
}

int	check_command_type(char	**envpc, char **command_args)
{
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
	int	i;

	i = 0;
	if (command_args)
	{
		while (command_args[i])
			i++;
	}
	return (i);
}

int	run_builtin(char ***envpc, char ***command_args_string, t_command_args ***command_args, int *fds)
{
	int	outcome;

	outcome = 0;
	if (check_command_type(*envpc, (*command_args_string)) == BUILTIN_ECHO)
		outcome = builtin_echo((*command_args_string));
	else if (check_command_type(*envpc, (*command_args_string)) == BUILTIN_CD)
		outcome = builtin_cd(&(*command_args_string)[1], envpc);
	else if (check_command_type(*envpc, (*command_args_string)) == BUILTIN_PWD)
		outcome = builtin_pwd(&(*command_args_string)[1], envpc);
	else if (check_command_type(*envpc, (*command_args_string)) == BUILTIN_EXPORT)
		outcome = builtin_export(&(*command_args_string)[1], envpc);
	else if (check_command_type(*envpc, (*command_args_string)) == BUILTIN_UNSET)
		outcome = builtin_unset((*command_args_string), envpc);
	else if (check_command_type(*envpc, (*command_args_string)) == BUILTIN_ENV)
		outcome = builtin_env((*command_args_string), envpc);
	else if (check_command_type(*envpc, (*command_args_string)) == BUILTIN_EXIT)
		builtin_exit_string(command_args_string, envpc, command_args, fds);
	return (outcome);
}

int		run_in_parent(t_command_args ***command_args, int index, char ***envpc, char ***command_args_string)
{
	int		outcome;
	int		fds[2];
	// int		actual_readfd;
	// int		actual_writefd;

	fds[0] = dup(STDIN_FILENO); // actual_readfd
	fds[1] = dup(STDOUT_FILENO); // actual_writefd
	if ((*command_args)[index]->writefd != STDOUT_FILENO)
		dup2((*command_args)[index]->writefd, STDOUT_FILENO);
	if ((*command_args)[index]->readfd != STDIN_FILENO)
		dup2((*command_args)[index]->readfd, STDIN_FILENO);
	outcome = run_builtin(envpc, command_args_string, command_args, fds);
	// close all pipes
	if ((*command_args)[index]->writefd != STDOUT_FILENO)
		close((*command_args)[index]->writefd);
	if ((*command_args)[index]->readfd != STDIN_FILENO)
		close((*command_args)[index]->readfd);
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
	return (outcome);
}

void	run_in_child(t_command_args ***command_args, int index, char ***envpc, char ***command_args_string)
{
	int	i;
	pid_t	pid;

	pid = fork();
	(*command_args)[index]->pid = pid;
	if (pid == 0)
	{
		if ((*command_args)[index]->writefd != STDOUT_FILENO)
			dup2((*command_args)[index]->writefd, STDOUT_FILENO);
		if ((*command_args)[index]->readfd != STDIN_FILENO)
			dup2((*command_args)[index]->readfd, STDIN_FILENO);
		// close all pipes
		i = 0;
		while ((*command_args)[i])
		{
			if ((*command_args)[i]->writefd != STDOUT_FILENO)
				close((*command_args)[i]->writefd);
			if ((*command_args)[i]->readfd != STDIN_FILENO)
				close((*command_args)[i]->readfd);
			i++;
		}
		// execute builtin
		builtin_exit(run_builtin(envpc, command_args_string, command_args, NULL));
		// run_builtin(envpc, command_args_string);
	}
}

void	execute_in_child(t_command_args ***command_args, int index, char ***envpc, char ***command_args_string)
{
	int	i;
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	(*command_args)[index]->pid = pid;
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL); // ctrl + c
		signal(SIGQUIT, SIG_DFL); // ctrl + slash
		if ((*command_args)[index]->writefd != STDOUT_FILENO)
			dup2((*command_args)[index]->writefd, STDOUT_FILENO);
		if ((*command_args)[index]->readfd != STDIN_FILENO)
			dup2((*command_args)[index]->readfd, STDIN_FILENO);
		// close all pipes
		i = 0;
		while ((*command_args)[i])
		{
			if ((*command_args)[i]->writefd != STDOUT_FILENO)
				close((*command_args)[i]->writefd);
			if ((*command_args)[i]->readfd != STDIN_FILENO)
				close((*command_args)[i]->readfd);
			i++;
		}
		// execute command
		if (check_command_type(*envpc, *command_args_string) == EXECUTABLE_PATH)
			execve(get_executable_in_path(*envpc, *command_args_string), *command_args_string, *envpc);
		else
			execve(*command_args_string[0], *command_args_string, *envpc);
		free(*command_args_string);
		freecommandlist(command_args);
		envpc_free(envpc);
		builtin_exit(0);
	}
	// free(command_args_string[0]);
}

void	update_question_mark(char ***envpc, int status, int last_status)
{
	char	*exitstring;

	if (status == 130)
	{
		exitstring = ft_itoa(130);
	}
	else if (status == 131)
	{
		exitstring = ft_itoa(131);
		ft_putstr_fd("Quit\n", 1);
	}
	else if (last_status != -999)
	{
		exitstring = ft_itoa(last_status);
	}
	else
	{
		exitstring = ft_itoa(status);
	}
	envpc_add(envpc, "?", exitstring);
	free(exitstring);	
}

void	execution(t_command_args ***command_args, char ***envpc)
{
	int i;
	char	**command_args_string;
	int		command_type;
	int		status;
	int		last_status;

	last_status = -999;
	i = 0;
	while ((*command_args)[i])
	{
		status = -999;
		command_type=0;
		if (count_commands_args((*command_args)[i]->tokenlist) == 0)
			(*command_args)[i]->cancelexec = 1;
		// printf("command_args[%d]->cancelexec = %d\n", i, command_args[i]->cancelexec);
		if ((*command_args)[i]->cancelexec == 0)
		{
			command_args_string = command_args_extraction((*command_args)[i]->tokenlist);
			command_type = check_command_type(*envpc, command_args_string);
			if (command_type  < 98)// builtin
			{
				// execute builtin
				if (command_args_len((*command_args)) == 1) // no pipes so run in parent
				{
					status = run_in_parent(command_args, i, envpc, &command_args_string);
				}
				else // pipes avail then run in child
					run_in_child(command_args, i, envpc, &command_args_string);
			}
			else  if (command_type == EXECUTABLE || command_type == EXECUTABLE_PATH)// executable
			{
				execute_in_child(command_args, i, envpc, &command_args_string);
			}
			else if (command_type == DIRECTORY)
			{
				ft_putstr_fd("minishell: ", 1);
				ft_putstr_fd(command_args_string[0], 2);
				ft_putstr_fd(": Is a directory\n", 2);
				status = 126;
			}
			else if (command_type == PERMISSION_DENIED)
			{
				ft_putstr_fd("minishell: ", 1);
				ft_putstr_fd(command_args_string[0], 2);
				ft_putstr_fd(": Permission denied\n", 2);
				status = 126;
			}
			else if (command_type == NOT_FOUND)
			{
				ft_putstr_fd(command_args_string[0], 2);
				ft_putstr_fd(": command not found\n", 2);
				status = 127;
			}
			else if (command_type == NO_SUCH_FILE_OR_DIRECTORY)
			{
				ft_putstr_fd("minishell: ", 1);
				ft_putstr_fd(command_args_string[0], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				status = 127;
			}
			free(command_args_string);
		}
		else
		{
			status = (*command_args)[i]->cancelexec % 2;
		}
		// close all the pipes used
		if ((*command_args)[i]->writefd != STDOUT_FILENO)
		{
			close((*command_args)[i]->writefd);
			(*command_args)[i]->writefd = STDOUT_FILENO;
		}
		if ((*command_args)[i]->readfd != STDIN_FILENO)
		{
			close((*command_args)[i]->readfd);
			(*command_args)[i]->readfd = STDIN_FILENO;
		}
		if ((*command_args)[i+1] == NULL && command_type != EXECUTABLE && command_type != EXECUTABLE_PATH)	
		{
			last_status = status;
		}
		i++;
	}
	// while (waitpid(-1, &status, 0) > 0)
	// ;
	i = 0;
	while ((*command_args)[i])
	{
		waitpid((*command_args)[i]->pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				ft_putstr_fd("\n", 1);
		}
		i++;
	}
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	update_question_mark(envpc, status, last_status);
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
