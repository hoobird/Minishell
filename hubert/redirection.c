// https://www.gnu.org/software/bash/manual/html_node/Redirections.html
//The following symbolic constants are provided for mode:
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

# include "minishell.h"

// redirection <
int	redirect_input(int fdRead,int fd2)
{
	if (dup2(fd2, fdRead) == -1)//redirect file to stdin
		printerror("redirect to stdin failed");
	close(fd2);
}

// redirection > and >>
int	redirect_output(int fdWrite, int fd2)
{
	if (dup2(fd2, fdWrite) == -1)
		printerror("redirect to stdOUT failed");
	close(fd2);
}

// HEREDOC <<
// HEREDOC must run before all other redirections (Based on XF)
int	redirect_heredoc(char *eof, int fdRead)
{
	char 	*line;
	int		pipes[2];

	pipe(pipes);
	line = readline("> ");
	while (line)
	{
		if (ft_strncmp(line, eof, ft_strlen(eof)) == 0)
			break ;
		ft_putstr_fd(line, pipes[1]);
		printf("line = %s\n", line);
		ft_putstr_fd("\n", pipes[1]);
		free(line);
		line = readline("> ");
	}
	free(line);
	close(pipes[1]);
	dup2(pipes[0], fdRead);
	close(pipes[0]);
}

// check for read / write permissions
// possible mode are R_OK, W_OK, X_OK, F_OK
// R_OK - check for read permission
// W_OK - check for write permission
// X_OK - check for execute permission (DONT NEED THIS FOR NOW)
// F_OK - check for file existence (DONT NEED THIS FOR NOW)
// return 0 if permission denied
// return 1 if permission granted
int	check_file_permissions(char *filename, int mode)
{
	if (access(filename, mode) == -1)
	{
		if (mode == R_OK)
			printerror("minishell: %s: No such file or directory\n");
		else if (mode == W_OK)
			printerror("minishell: %s: Permission denied\n");
		return (0);
	}
	return (1);
}

// redirection >  be 41
// redirection >> be 42
// redirection <  be 43
// redirection << be 44
// ---     ---     ---
// rwx     rwx     rwx
// user    group   other 

// perfrom redirections as per token type
void	perform_redirection(t_command_args **command_args)
{
	int	i;
	t_token	*tokens;
	int	result;

	i = 0;
	while (command_args[i])
	{
		tokens = command_args[i]->tokenlist;
		while (tokens)
		{
			// check permission first
			if (tokens->type == RE_OUTPUT || tokens->type == RE_APPEND) // > and >> need write permission
				result = check_file_permissions(tokens->string, W_OK);
			else if (tokens->type == RE_INPUT) // < need read permission
				result = check_file_permissions(tokens->string, R_OK);
			else
			{
				tokens = tokens->next;
				continue ;
			}
			if (result == 0) // permission denied
			{
				command_args[i]->cancelexec = 1; // cancel execution once redirection fails
				break ;
			}
			// perform redirection
			if (tokens->type == RE_OUTPUT)
				redirect_output(command_args[i]->writefd, open(tokens->string, O_WRONLY | O_CREAT, 0644));
			else if (tokens->type == RE_APPEND)
				redirect_output(command_args[i]->writefd, open(tokens->string, O_WRONLY | O_APPEND | O_CREAT, 0644));
			else if (tokens->type == RE_INPUT)
				redirect_input(command_args[i]->readfd, open(tokens->string, O_RDONLY));
			else if (tokens->type == RE_HEREDOC)
				redirect_heredoc(tokens->string, command_args[i]->readfd);
			tokens = tokens->next;	
		}
		i++;
	}
}



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
// 			fd2 = open(fileright, O_WRONLY | O_CREAT, 0644); // if file dont exist, create with permission -rw-r--r--
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
// 			fd2 = open(fileright, O_WRONLY | O_APPEND | O_CREAT, 0644); // if file dont exist, create with permission -rw-r--r--
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