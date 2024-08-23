/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:52:18 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 18:07:14 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands_args(t_token *tokens)
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
	int	i;

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
	int	i;

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
	int	i;

	i = 0;
	while (array[i])
	{
		ft_putstr_fd(array[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	ft_putstr_fd("\n", 1);
}

void	join_paths(int i, char **path_list,
			char **command_args, char **binary_path)
{
	char	*paths;

	paths = ft_strjoin(path_list[i], "/");
	*binary_path = ft_strjoin(paths, command_args[0]);
	free(paths);
}

void	free_path_and_binary_path(char **binary_path, char ***path_list)
{
	path_list_free(*path_list);
	free(*binary_path);
}

int	check_executable_in_path(char **envpc, char **command_args)
{
	char	*paths;
	char	**path_list;
	int		i;
	char	*binary_path;

	paths = envpc_get_value(envpc, "PATH");
	if (paths != NULL)
	{
		path_list = ft_split(paths, ':');
		i = 0;
		while (path_list[i])
		{
			join_paths(i, path_list, command_args, &binary_path);
			if (check_file_type(binary_path) == 1
				&& check_file_permissions(binary_path, X_OK) == 1)
			{
				free_path_and_binary_path(&binary_path, &path_list);
				return (EXECUTABLE_PATH);
			}
			free(binary_path);
			i++;
		}
		path_list_free(path_list);
	}
	return (NOT_FOUND);
}

char	*get_exe_in_path(char **envpc, char **command_args)
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
		if (check_file_type(binary_path) == 1
			&& check_file_permissions(binary_path, X_OK) == 1)
		{
			break ;
		}
		free(binary_path);
		i++;
	}
	path_list_free(path_list);
	return (binary_path);
}

int	check_executable(char **envpc, char **command_args)
{
	if (ft_strchr(command_args[0], '/') == NULL)
	{
		if (check_executable_in_path(envpc, command_args) == EXECUTABLE_PATH)
			return (EXECUTABLE_PATH);
		else
			return (NOT_FOUND);
	}
	if (check_file_permissions(command_args[0], F_OK) == 0)
		return (NO_SUCH_FILE_OR_DIRECTORY);
	if (check_file_type(command_args[0]) == 2)
		return (DIRECTORY);
	if (check_file_type(command_args[0]) == 1)
	{
		if (check_file_permissions(command_args[0], X_OK) == 1)
			return (EXECUTABLE);
		else
			return (PERMISSION_DENIED);
	}
	return (NOT_FOUND);
}

int	check_cmd_type(char	**envpc, char **command_args)
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

int	command_args_len(t_command_args **command_args)
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

int	run_builtin(char ***envpc, char ***command_args_string,
		t_command_args ***command_args, int *fds)
{
	int	outcome;

	outcome = 0;
	if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_ECHO)
		outcome = builtin_echo((*command_args_string));
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_CD)
		outcome = builtin_cd(&(*command_args_string)[1], envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_PWD)
		outcome = builtin_pwd(&(*command_args_string)[1], envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_EXPORT)
		outcome = builtin_export(&(*command_args_string)[1], envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_UNSET)
		outcome = builtin_unset((*command_args_string), envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_ENV)
		outcome = builtin_env((*command_args_string), envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_EXIT)
		builtin_exit_string(command_args_string, envpc, command_args, fds);
	return (outcome);
}

int	run_in_parent(t_command_args ***command_args, int index,
		char ***envpc, char ***command_args_string)
{
	int		outcome;
	int		fds[2];

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if ((*command_args)[index]->writefd != STDOUT_FILENO)
		dup2((*command_args)[index]->writefd, STDOUT_FILENO);
	if ((*command_args)[index]->readfd != STDIN_FILENO)
		dup2((*command_args)[index]->readfd, STDIN_FILENO);
	outcome = run_builtin(envpc, command_args_string, command_args, fds);
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

void	run_in_child(t_command_args ***command_args, int index,
			char ***envpc, char ***command_args_string)
{
	int		i;
	pid_t	pid;

	pid = fork();
	(*command_args)[index]->pid = pid;
	if (pid == 0)
	{
		if ((*command_args)[index]->writefd != STDOUT_FILENO)
			dup2((*command_args)[index]->writefd, STDOUT_FILENO);
		if ((*command_args)[index]->readfd != STDIN_FILENO)
			dup2((*command_args)[index]->readfd, STDIN_FILENO);
		i = 0;
		while ((*command_args)[i])
		{
			if ((*command_args)[i]->writefd != STDOUT_FILENO)
				close((*command_args)[i]->writefd);
			if ((*command_args)[i]->readfd != STDIN_FILENO)
				close((*command_args)[i]->readfd);
			i++;
		}
		exit(run_builtin(envpc, command_args_string, command_args, NULL));
	}
}

void	execute_in_child_signal(char p_or_c)
{
	if (p_or_c == 'p')
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (p_or_c == 'c')
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}

void	close_writefd_readfd(t_command_args **command_args, int i)
{
	if (command_args[i]->writefd != STDOUT_FILENO)
		close(command_args[i]->writefd);
	if (command_args[i]->readfd != STDIN_FILENO)
		close(command_args[i]->readfd);
}

void	execute_in_child(t_command_args **command_args, int index,
			char ***envpc, char **cmd_args_str)
{
	int		i;

	execute_in_child_signal('p');
	command_args[index]->pid = fork();
	if (command_args[index]->pid == 0)
	{
		execute_in_child_signal('c');
		if (command_args[index]->writefd != STDOUT_FILENO)
			dup2(command_args[index]->writefd, STDOUT_FILENO);
		if (command_args[index]->readfd != STDIN_FILENO)
			dup2(command_args[index]->readfd, STDIN_FILENO);
		i = 0;
		while (command_args[i])
		{
			close_writefd_readfd(command_args, i);
			i++;
		}
		if (check_cmd_type(*envpc, cmd_args_str) == EXECUTABLE_PATH)
			execve(get_exe_in_path(*envpc, cmd_args_str), cmd_args_str, *envpc);
		else
			execve(cmd_args_str[0], cmd_args_str, *envpc);
		printerror("execve failed\n");
		exit(127);
	}
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

void	handle_command_errors(int *status, int command_type, char **command_args_string)
{
	if (command_type == DIRECTORY)
	{
		ft_putstr_fd("minishell: ", 1);
		ft_putstr_fd(command_args_string[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		*status = 126;
	}
	else if (command_type == PERMISSION_DENIED)
	{
		ft_putstr_fd("minishell: ", 1);
		ft_putstr_fd(command_args_string[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		*status = 126;
	}
	else if (command_type == NOT_FOUND)
	{
		ft_putstr_fd(command_args_string[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		*status = 127;
	}
	else if (command_type == NO_SUCH_FILE_OR_DIRECTORY)
	{
		ft_putstr_fd("minishell: ", 1);
		ft_putstr_fd(command_args_string[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		*status = 127;
	}
}

void	execution_close_fds(t_command_args ***command_args, int i)
{
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
}

void	wait_childs_update_status(t_command_args ***command_args, int *status)
{
	int		i;
	
	i = 0;
	while ((*command_args)[i])
	{
		waitpid((*command_args)[i]->pid, status, 0);
		if (WIFSIGNALED(*status))
			if (WTERMSIG(*status) == SIGINT)
				ft_putstr_fd("\n", 1);
		i++;
	}
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		*status = WTERMSIG(*status) + 128;
}

typedef struct s_execution
{
	t_command_args	***command_args;
	char			***envpc;
	
}	t_execution;

void	execution(t_command_args ***command_args, char ***envpc)
{
	int		i;
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
		if ((*command_args)[i]->cancelexec == 0)
		{
			command_args_string = command_args_extraction((*command_args)[i]->tokenlist);
			command_type = check_cmd_type(*envpc, command_args_string);
			if (command_type  < 98)
			{
				if (command_args_len((*command_args)) == 1)
					status = run_in_parent(command_args, i, envpc, &command_args_string);
				else
					run_in_child(command_args, i, envpc, &command_args_string);
			}
			else  if (command_type == EXECUTABLE || command_type == EXECUTABLE_PATH)
				execute_in_child((*command_args), i, envpc, command_args_string);
			else
				handle_command_errors(&status, command_type, command_args_string);
			free(command_args_string);
		}
		else
			status = (*command_args)[i]->cancelexec % 2;
		execution_close_fds(command_args, i);
		if ((*command_args)[i+1] == NULL && command_type != EXECUTABLE && command_type != EXECUTABLE_PATH)
			last_status = status;
		i++;
	}
	wait_childs_update_status(command_args, &status);
	update_question_mark(envpc, status, last_status);
}
