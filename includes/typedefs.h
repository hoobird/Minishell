/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:25:15 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:53:09 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

typedef enum e_tokentype
{
	WORD = 1,
	SQUOTE = 2,
	DQUOTE = 3,
	RE_OUTPUT = 41,
	RE_APPEND = 42,
	RE_INPUT = 43,
	RE_HEREDOC = 44,
	RE_HEREDOC_QUOTED = 45,
	PIPE = 5,
	COMMAND = 6,
	ARGS = 7,
	ERROR_UNCLOSED_QUOTES = 90,
}					t_tokentype;

typedef struct s_token
{
	char			*string;
	t_tokentype		type;
	int				postspace;
	struct s_token	*next;
}					t_token;

typedef struct s_command_args
{
	t_token			*tokenlist;
	int				readfd;
	int				writefd;
	int				cancelexec;
	pid_t			pid;
}					t_command_args;

typedef enum e_command_type
{
	BUILTIN_ECHO = 91,
	BUILTIN_CD = 92,
	BUILTIN_PWD = 93,
	BUILTIN_EXPORT = 94,
	BUILTIN_UNSET = 95,
	BUILTIN_ENV = 96,
	BUILTIN_EXIT = 97,
	EXECUTABLE = 98,
	EXECUTABLE_PATH = 980,
	DIRECTORY = 981,
	PERMISSION_DENIED = 982,
	NO_SUCH_FILE_OR_DIRECTORY = 983,
	NOT_FOUND = 99,
}					t_command_type;

typedef struct s_execution
{
	t_command_args	***cmd_args;
	char			***envpc;
	int				*status;
	int				*command_type;
}					t_execution;

typedef struct s_redirection
{
	t_tokentype		type;
	char			*fileeof;
	int				fd;
}					t_redirection;

typedef struct s_minishell
{
	char			**envpc;
	char			*buffer;
	t_token			**tokenlistlist;
	t_command_args	**cmdargslist;
}					t_minishell;

typedef struct s_pq_helper
{
	t_token			*token;
	int				quoted;
	int				start;
	int				i;
}					t_pq_helper;

#endif
