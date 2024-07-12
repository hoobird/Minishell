/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:21:39 by hulim             #+#    #+#             */
/*   Updated: 2024/07/08 17:41:43 by hulim            ###   ########.fr       */
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
// For opendir, readdir, closedir
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

typedef enum	e_tokentype
{
	command = 1,
	re_output = 41,	// redirection >  be 41
	re_append = 42,	// redirection >> be 42
	re_input = 43,	// redirection <  be 43
	re_heredoc = 44	// redirection << be 44
}				t_tokentype;

// perror.c
int	printerror(char	*errormsg);


typedef struct s_token
{
	char	*token;
	t_tokentype type;
}			t_token;


# endif