// https://www.gnu.org/software/bash/manual/html_node/Redirections.html
// The following symbolic constants are provided for mode:
// S_IRWXU  00700 user (file owner) has read, write, and execute permission
// S_IRUSR  00400 user has read permission
// S_IWUSR  00200 user has write permission
// S_IXUSR  00100 user has execute permission
// S_IRWXG  00070 group has read, write, and execute permission
// S_IRGRP  00040 group has read permission
// S_IWGRP  00020 group has write permission
// S_IXGRP  00010 group has execute permission
// S_IRWXO  00007 others have read, write, and execute permission
// S_IROTH  00004 others have read permission
// S_IWOTH  00002 others have write permission
// S_IXOTH  00001 others have execute permission

// According to POSIX, the effect when other bits are set in mode is unspecified.  On Linux, the following bits are also honored in mode:
// S_ISUID  0004000 set-user-ID bit
// S_ISGID  0002000 set-group-ID bit (see inode(7)).
// S_ISVTX  0001000 sticky bit (see inode(7)).

#include "minishell.h"

typedef	struct	s_redirection
{
	t_tokentype		type;
	char			*fileeof;
	int				fd;
}				t_redirection;

int		count_redirection_cmd_args(t_command_args *command_args)
{
	int		count;
	t_token	*tokens;

	count = 0;
	tokens = command_args->tokenlist;
	while (tokens)
	{
		if (tokens->type == RE_OUTPUT || tokens->type == RE_APPEND || tokens->type == RE_INPUT || tokens->type == RE_HEREDOC)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_redirection	**setup_redirectionlist(t_command_args **command_args)
{
	int				i;
	int				j;
	t_token			*tokens;
	t_redirection	**redirectionlist;

	redirectionlist = ft_calloc(sizeof(t_redirection *), (command_args_len(command_args) + 1));
	i = 0;
	while (command_args[i])
	{
		tokens = command_args[i]->tokenlist;
		redirectionlist[i] = ft_calloc(sizeof(t_redirection), (count_redirection_cmd_args(command_args[i]) + 1));
		j = 0;
		while (tokens)
		{
			if (tokens->type == RE_OUTPUT || tokens->type == RE_APPEND || tokens->type == RE_INPUT || tokens->type == RE_HEREDOC)
			{
				redirectionlist[i][j].type = tokens->type;
				redirectionlist[i][j].fileeof = ft_strdup(tokens->string);
				redirectionlist[i][j].fd = -1; 
				j++;
			}				
			tokens = tokens->next;
		}	
		i++;
	}
	return (redirectionlist);
}

void	free_redirectionlist(t_redirection **redirectionlist)
{
	int	i;
	int	j;

	i = 0;
	while (redirectionlist[i])
	{
		j = 0;
		while (redirectionlist[i][j].type)
		{
			free(redirectionlist[i][j].fileeof);
			j++;
		}
		free(redirectionlist[i]);
		i++;
	}
	free(redirectionlist);
}

// HEREDOC <<
// HEREDOC must run before all other redirections (Based on XF)
int	redirect_heredoc(char *eof)
{
	char	*line;
	int		pipes[2];
	pid_t	pid;

	pipe(pipes);
	pid = fork();
	if (pid == 0)
	{
		line = readline("> ");
		while (line)
		{
			if (ft_strncmp(line, eof, ft_strlen(eof)) == 0)
				break ;
			ft_putstr_fd(line, pipes[1]);
			ft_putstr_fd("\n", pipes[1]);
			free(line);
			line = readline("> ");
		}
		free(line);
		close(pipes[1]);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	close(pipes[1]);
	return (pipes[0]);
}

void	redirect_heredoc_first(t_redirection **redirectionlist)
{
	int		i;
	int		j;
	
	i = 0;
	while (redirectionlist[i])
	{
		j = 0;
		while (redirectionlist[i][j].type)
		{
			if (redirectionlist[i][j].type == RE_HEREDOC)
			{
				// perform redirection
				redirectionlist[i][j].fd = redirect_heredoc(redirectionlist[i][j].fileeof);
			}
			j++;
		}
		i++;
	}

}
// check file permissions
// 0 - permission denied
// 1 - OK
// 2 - No such file or directory
// 4 - file is a directory
int	handle_output_append(t_redirection *redir)
{
	int		status;

	status = 1;
	if (check_file_permissions(redir->fileeof, F_OK) == 1)
	{
		if (check_file_type(redir->fileeof) == 2)
			return (4);
		if (check_file_permissions(redir->fileeof, W_OK) == 0)
			return (0);
	}
	else
	{
		if (redir->type == RE_OUTPUT)
			redir->fd = open(redir->fileeof, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == RE_APPEND)
			redir->fd = open(redir->fileeof, O_WRONLY | O_APPEND | O_CREAT, 0644);
	}
	return (status);
}

int	handle_input(t_redirection *redir)
{
	int		status;

	status = 1;
	if (check_file_permissions(redir->fileeof, F_OK) == 0)
		return (2);
	if (check_file_type(redir->fileeof) == 2)
		return (4);
	if (check_file_permissions(redir->fileeof, R_OK) == 0)
		return (0);
	if (status == 1)
		redir->fd = open(redir->fileeof, O_RDONLY);
	return (status);
}

void	redir_print_fail(int status, char *fileeof)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(fileeof, 2);
	if (status == 0)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (status == 2)
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (status == 4)
		ft_putstr_fd(": Is a directory\n", 2);
}

void	redirect_rest_later(t_redirection **redirectionlist, t_command_args **command_args)
{
	int		i;
	int		j;
	int		status;

	i = 0;
	status = 1;
	while (redirectionlist[i])
	{
		j = 0;
		while (redirectionlist[i][j].type)
		{
			if (redirectionlist[i][j].type == RE_OUTPUT || redirectionlist[i][j].type == RE_APPEND)
				status = handle_output_append(&redirectionlist[i][j]);
			else if (redirectionlist[i][j].type == RE_INPUT)
				status = handle_input(&redirectionlist[i][j]);
			if (status == 0 || status == 2 || status == 4)
			{
				redir_print_fail(status, redirectionlist[i][j].fileeof);
				command_args[i]->cancelexec = 1; // cancel execution once redirection fails
				break ;
			}
			j++;
		}
		i++;
	}
}

void	assignreadwritefd(t_command_args **command_args, t_redirection **redirectionlist)
{
	int		i;
	int		j;
	int 	original_readfd;
	int		original_writefd;

	i = 0;
	while (command_args[i])
	{
		j = 0;
		original_readfd = command_args[i]->readfd;
		original_writefd = command_args[i]->writefd;
		while (redirectionlist[i][j].fd)
		{
			if (redirectionlist[i][j].fd == -1)
			{
				j++;
				continue;
			}
			if (redirectionlist[i][j].type == RE_OUTPUT || redirectionlist[i][j].type == RE_APPEND)
				command_args[i]->writefd = redirectionlist[i][j].fd;
			else if (redirectionlist[i][j].type == RE_INPUT || redirectionlist[i][j].type == RE_HEREDOC)
				command_args[i]->readfd = redirectionlist[i][j].fd;
			j++;
		}
		if (original_readfd != 0 && original_readfd != command_args[i]->readfd)
			close(original_readfd);
		if (original_writefd != 1 && original_writefd != command_args[i]->writefd)
			close(original_writefd);
		i++;
	}
}

void	closeunusedfd(t_redirection **redirectionlist, t_command_args **command_args)
{
	int		i;
	int		j;

	i = 0;
	while (redirectionlist[i])
	{
		j = 0;
		while (redirectionlist[i][j].type)
		{
			if (redirectionlist[i][j].fd != -1 && redirectionlist[i][j].fd != command_args[i]->readfd && redirectionlist[i][j].fd != command_args[i]->writefd)
				close(redirectionlist[i][j].fd);
			j++;
		}
		i++;
	}
}

void	perform_redirection(t_command_args **command_args)
{
	t_redirection	**redirectionlist;

	redirectionlist = setup_redirectionlist(command_args);
	// perform redirection
	redirect_heredoc_first(redirectionlist);
	redirect_rest_later(redirectionlist, command_args);
	assignreadwritefd(command_args, redirectionlist);
	closeunusedfd(redirectionlist, command_args);
	free_redirectionlist(redirectionlist);
}

// // redirection <
// void	redirect_input(int *fdRead, int fd2)
// {
// 	*fdRead = fd2;
// }

// // redirection > and >>
// void	redirect_output(int *fdWrite, int fd2)
// {
// 	*fdWrite = fd2;
// }

// // HEREDOC <<
// // HEREDOC must run before all other redirections (Based on XF)
// int	redirect_heredoc(char *eof)
// {
// 	char	*line;
// 	int		pipes[2];

// 	pipe(pipes);
// 	line = readline("> ");
// 	while (line)
// 	{
// 		if (ft_strncmp(line, eof, ft_strlen(eof)) == 0)
// 			break ;
// 		ft_putstr_fd(line, pipes[1]);
// 		printf("line = %s\n", line);
// 		ft_putstr_fd("\n", pipes[1]);
// 		free(line);
// 		line = readline("> ");
// 	}
// 	free(line);
// 	close(pipes[1]);
// 	return (pipes[0]);
// }

// void	handle_heredoc(int signal)
// {
// 	g_received_signal = signal;
// }

// void	perform_heredoc_first(t_command_args **command_args)
// {
// 	int		i;
// 	t_token	*tokens;
// 	int		actual_readfd;

// 	if (fork() == 0)
// 	{
// 		signal(SIGINT, handle_heredoc);
// 		signal(SIGQUIT, handle_heredoc);
// 		i = 0;
// 		while (command_args[i])
// 		{
// 			tokens = command_args[i]->tokenlist;
// 			actual_readfd = command_args[i]->readfd;
// 			while (tokens)
// 			{
// 				if (tokens->type == RE_HEREDOC)
// 				{
// 					if (actual_readfd != 0)
// 						close(actual_readfd);
// 					actual_readfd = redirect_heredoc(tokens->string);
// 				}
// 				tokens = tokens->next;
// 			}
// 			command_args[i]->readfd = actual_readfd;
// 			i++;
// 		}
// 		exit(0);
// 		fd_putstr_fd("heredoc child did not die\n", 2);
// 	}

// }

// // redirection >  be 41
// // redirection >> be 42
// // redirection <  be 43
// // redirection << be 44
// // ---     ---     ---
// // rwx     rwx     rwx
// // user    group   other

// // perfrom redirections as per token type
// void	perform_redirection(t_command_args **command_args)
// {
// 	int		i;
// 	t_token	*tokens;
// 	int		result;

// 	i = 0;
// 	result = 1;
// 	perform_heredoc_first(command_args);
// 	while (command_args[i])
// 	{
// 		tokens = command_args[i]->tokenlist;
// 		while (tokens)
// 		{
// 			// check permission first
// 			if (tokens->type == RE_OUTPUT || tokens->type == RE_APPEND) // > and >> need write permission
// 			{
// 				if (check_file_permissions(tokens->string, F_OK) == 1)
// 					result = check_file_permissions(tokens->string, W_OK);
// 			}
// 			else if (tokens->type == RE_INPUT) // < need read permission and need to exist
// 			{
// 				if (check_file_permissions(tokens->string, F_OK) == 1 && check_file_type(tokens->string) == 1)
// 					result = check_file_permissions(tokens->string, R_OK);
// 				else
// 					result = 2; // file does not exist
// 				// printf("result = %d\n", result);
// 			}
// 			else if (tokens->type != RE_HEREDOC)
// 			{
// 				tokens = tokens->next;
// 				continue ;
// 			}
// 			if (result == 0 || result == 2) // permission denied or file does not exist
// 			{
// 				command_args[i]->cancelexec = (result + 1) % 2; // cancel execution once redirection fails
// 				ft_putstr_fd("minishell: ", 2);
// 				ft_putstr_fd(tokens->string, 2);
// 				if (result == 0)
// 					ft_putstr_fd(": Permission denied\n", 2);
// 				else if (result == 2)
// 					ft_putstr_fd(": No such file or directory\n", 2);
// 				break ;
// 			}

// 			// perform redirection
// 			if (tokens->type == RE_OUTPUT)
// 			{
// 				result = open(tokens->string, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 				redirect_output(&(command_args[i]->writefd), result);
// 			}
// 			else if (tokens->type == RE_APPEND)
// 			{
// 				result = open(tokens->string, O_WRONLY | O_APPEND | O_CREAT, 0644);
// 				redirect_output(&(command_args[i]->writefd), result);
// 			}
// 			else if (tokens->type == RE_INPUT)
// 				redirect_input(&(command_args[i]->readfd), open(tokens->string, O_RDONLY));
// 			if (result < 0)
// 			{
// 				ft_putstr_fd("minishell: ", 2);
// 				ft_putstr_fd(tokens->string, 2);
// 				ft_putstr_fd(": No such file or directory\n", 2);
// 				break;
// 			}
// 			tokens = tokens->next;
// 		}
// 		i++;
// 	}
// }

// Old test cases etc

// int	redirection(t_tokentype tnum, char *fileleft, char *fileright)
// {
// 	int	fd1;
// 	int	fd2;

// 	if (tnum == 41) // Read left, write right
// 	{
// 		fd1 = STDIN_FILENO;
// 		if (fileleft)
// 			fd1 = open(fileleft, O_RDONLY);
// 		if (fileright)
// 			fd2 = open(fileright, O_WRONLY | O_CREAT, 0644);
				// if file dont exist, create with permission -rw-r--r--
// 		if (fd1 < 0 || fd2 < 0)
// 			return(printerror("redirection > : file open() failed\n"));
// 		redirect_output(fd1, fd2);
// 		if (fd1 != STDIN_FILENO)
// 			close(fd1);
// 	}
// 	else if (tnum == 42)
// 	{
// 		fd1 = STDIN_FILENO;
// 		if (fileleft)
// 			fd1 = open(fileleft, O_RDONLY);
// 		if (fileright)
// 			fd2 = open(fileright, O_WRONLY | O_APPEND | O_CREAT, 0644);
				// if file dont exist, create with permission -rw-r--r--
// 		if (fd1 < 0 || fd2 < 0)
// 			return(printerror("redirection >> : file open() failed\n"));
// 		redirect_output(fd1, fd2);
// 		if (fd1 != STDIN_FILENO)
// 			close(fd1);
// 	}
// 	else if (tnum == 43)
// 	{
// 			fd2 = open(fileright, O_RDONLY);
// 			if (fd2 < 0)
// 				return(printerror("redirection < : file open() failed\n"));
// 			redirect_input(fd2);
// 	}
// 	else if (tnum == 44)
// 	{
// 		// in this case, fileright is not a file but the delimiter
// 		redirect_heredoc(fileright);
// 	}
// }

// // rest redirect <
// int main()
// {
// 	// char *filename = "testfile.txt";
// 	char *arg[3] = {"cat", NULL, NULL};
// 	t_tokentype tnum = 44;

// 	redirection(tnum, NULL, "eof");
// 	if (fork() == 0)
// 		execve("/bin/cat", arg, NULL);
// 	waitpid(-1, NULL, 0);
// 	return (0);
// }

// // rest redirect >> and >
// int main()
// {
// 	char *filename = "./test/file1.txt";
// 	char *filename2 = "./test/file3.txt";
// 	char *filename3 = "./test/file2.txt";
// 	char *arg[3] = {"cat", filename, NULL};
// 	t_tokentype tnum = 41;

// 	redirection(tnum, filename, filename2);
// 	redirection(tnum, filename2, filename3);
// 	// if (fork() == 0)
// 	// 	execve("/bin/cat", arg, NULL);
// 	return (0);
// }