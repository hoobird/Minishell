/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:21:39 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 16:56:36 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "minishell>> "

// For printf, perror
# include <stdio.h>
// For malloc, free, exit, getenv
# include <stdlib.h>
// For write, access, open, read, close, fork, getcwd, chdir, unlink, execve,
// dup, dup2, pipe, isatty, ttyname, ttyslot
# include <unistd.h>
// For open (for file control options)
# include <fcntl.h>
// For wait, waitpid, wait3, wait4
# include <sys/wait.h>
// For signal, sigaction, sigemptyset, sigaddset, kill
# include <signal.h>
// For stat, lstat, fstat
# include <sys/stat.h>
// For mask values stat's st_mode to check file type
// (directory, regular file, etc)
# include <dirent.h>
// For strerror
# include <string.h>
// For ioctl
# include <sys/ioctl.h>
// For tcsetattr, tcgetattr
# include <termios.h>
// For tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
// (or <term.h> if your system uses it)
# include <curses.h>
// For readline, rl_clear_history, rl_on_new_line,
// rl_replace_line, rl_redisplay
# include <readline/readline.h>
// For add_history
# include <readline/history.h>
// For libft
# include "libft.h"
# include "typedefs.h"
// for builtins like env echo cd pwd export unset exit
# include "builtin.h"
// For parsing
# include "execution.h"
# include "parsing.h"

extern int		g_received_signal;

// printerror.c
int				printerror(char *errormsg);
void			printerror_heredoc_eof(char *eof);
int				printerror_unexpected_pipetoken();
int				printerror_unexpected_token(t_token *nexttoken);


// piping.c
void			printpipelist(int **pipelist);
t_command_args	**upgrade_struct_generate_pipes(t_token **tokenlistlist);
void			printcommandlist(t_command_args **command_arg_list);

// redirection.c redirection2.c redirection3.c redirection4.c
int				count_redir(t_command_args *command_args);
void			add_to_redir_list(t_redirection *redir, t_token *tokens);
t_redirection	**setup_redirectionlist(t_command_args **cmd_arg);
void			free_redirectionlist(t_redirection **redirectionlist);
void			handle_heredoc(int signal);
void			readline_heredoc(t_redirection red, char ***envpc, int *pipes);
int				redirect_heredoc(t_redirection **redlist, t_redirection red,
					char ***envpc, t_command_args ***command_args);
int				handle_output_append(t_redirection *redir);
int				handle_input(t_redirection *redir);
void			redir_print_fail(int status, char *fileeof);
void			redirect_heredoc_first(t_redirection **redirectionlist, char ***envpc,
					t_command_args ***command_args);
void			redirect_rest_later(t_redirection **rl,
					t_command_args **command_args);
void			update_fds(t_redirection **rl, int i, int j,
					t_command_args **cmd_args);
void			close_original_fds(int original_readfd,
					t_command_args **cmd_args, int i, int original_writefd);
void			assignreadwritefd(t_command_args **cmd_args,
					t_redirection **rl);
void			closeunusedfd(t_redirection **redirlist,
					t_command_args **command_args);
void			cancel_all_exec(t_command_args **command_args);
int				perform_redirection(t_command_args ***command_args,
					char ***envpc, char **buffer);

// execute.c
void			execution(t_command_args ***command_args, char ***envpc);
void			free_command_args(t_command_args **command_args);
int				command_args_len(t_command_args **command_args);

// check_file_status.c
int				check_file_type(char *path);
int				check_file_permissions(char *filename, int mode);

// main.c mainhelper.c mainhelper2.c
void			freecommandlist(t_command_args ***command_args_list);
void			handle_readline(int sig);
int				is_not_empty_string(char *str);
int				main_exit_free_envpc(char ***envpc);
void			startup(char **envp, char ***envpc, int argc, char *argv[]);
void			main_update_sigint_envpc(char ***envpc);
void			free_cmd_args_buffer(t_command_args ***command_args_list,
					char **buffer);
int				add_hist_or_bye(char **buffer);
int				check_parse_fail(t_token ***tokenlistlist, char **buffer,
					char ***envpc);
int				check_buffer_eof(char **buffer);
#endif