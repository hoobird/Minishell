/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:25:15 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 20:53:43 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

typedef enum e_tokentype
{
	WORD = 1,
	SQUOTE = 2,
	DQUOTE = 3,
	RE_OUTPUT = 41,	// redirection >  be 41
	RE_APPEND = 42,	// redirection >> be 42
	RE_INPUT = 43,	// redirection <  be 43
	RE_HEREDOC = 44,	// redirection << be 44
	RE_HEREDOC_QUOTED = 45,	// redirection with quoted EOF << be 45
	PIPE = 5,
	COMMAND = 6,
	ARGS = 7,
	ERROR_UNCLOSED_QUOTES = 90,
}				t_tokentype;

typedef struct s_token
{
	char				*string; /* Zero terminated string. */
	t_tokentype			type;	/* Flags associated with this word. */
	int					postspace; /*0 if no space after,1 if got space after*/
	struct s_token		*next;
}				t_token;

typedef struct s_command_args
{
	t_token	*tokenlist;
	int		readfd;
	int		writefd;
	int		cancelexec;
	pid_t	pid;
}			t_command_args;

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
	EXECUTABLE_PATH = 980	,
	DIRECTORY = 981,
	PERMISSION_DENIED = 982,
	NO_SUCH_FILE_OR_DIRECTORY = 983,
	NOT_FOUND = 99,
}				t_command_type;

typedef struct s_execution
{
	t_command_args	***cmd_args;
	char			***envpc;
	int				*status;
	int				*command_type;
}	t_execution;

#endif
