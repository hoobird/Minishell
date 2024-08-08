/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:21:39 by hulim             #+#    #+#             */
/*   Updated: 2024/08/07 23:05:50 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "minishell>> "

// For printf, perror
#include <stdio.h>        
// For malloc, free, exit, getenv
#include <stdlib.h>       
// For write, access, open, read, close, fork, getcwd, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot
#include <unistd.h>       
// For open (for file control options)
#include <fcntl.h>        
// For wait, waitpid, wait3, wait4
#include <sys/wait.h>     
// For signal, sigaction, sigemptyset, sigaddset, kill
#include <signal.h>       
// For stat, lstat, fstat
#include <sys/stat.h>
// For mask values stat's st_mode to check file type (directory, regular file, etc)
#include <dirent.h>       
// For strerror
#include <string.h>       
// For ioctl
#include <sys/ioctl.h>    
// For tcsetattr, tcgetattr
#include <termios.h>      
// For tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs (or <term.h> if your system uses it)
#include <curses.h>       
// For readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay
#include <readline/readline.h> 
// For add_history
#include <readline/history.h> 
// For libft
#include "libft.h"
// For parsing
#include "parsing.h"
// for builtins like env echo cd pwd export unset exit
#include "builtin.h"


// printerror.c
int	printerror(char	*errormsg);

typedef struct	s_command_args
{
	t_token	*tokenlist;
	int		readfd;
	int		writefd;
	int		cancelexec;
}			t_command_args;

typedef enum	e_command_type
{
	BUILTIN_ECHO =91,
	BUILTIN_CD = 92,
	BUILTIN_PWD = 93,
	BUILTIN_EXPORT = 94,
	BUILTIN_UNSET = 95,
	BUILTIN_ENV = 96,
	BUILTIN_EXIT = 97,
	EXECUTABLE = 98,
	DIRECTORY = 981,
	PERMISSION_DENIED = 982,
	NO_SUCH_FILE_OR_DIRECTORY = 983,
	NOT_FOUND = 99,
}				t_command_type;

// piping.c
void    		printpipelist(int **pipelist);
t_command_args	**upgrade_struct_generate_pipes(t_token **tokenlistlist);
void    printcommandlist(t_command_args **command_arg_list);

// redirection.c
void	perform_redirection(t_command_args **command_args);

// execute.c
void	execution(t_command_args **command_args, char ***envpc);
void	free_command_args(t_command_args **command_args);

// check_file_status.c
int	check_file_type(char *path);
int	check_file_permissions(char *filename, int mode);
	

# endif