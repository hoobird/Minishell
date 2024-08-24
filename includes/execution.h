/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:03:54 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:53:33 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

int			count_commands_args(t_token *tokens);
char		**command_args_extraction(t_token *tokens);
void		free_command_args(t_command_args **command_args);
void		path_list_free(char **path_list);
void		print_char_2d_array(char **array);
void		join_paths(int i, char **path_list, char **command_args,
				char **binary_path);
void		free_path_and_binary_path(char **binary_path, char ***path_list);
int			check_executable_in_path(char **envpc, char **command_args);
char		*get_exe_in_path(char **envpc, char **command_args);
int			check_executable(char **envpc, char **command_args);
int			check_cmd_type(char **envpc, char **command_args);
int			command_args_len(t_command_args **command_args);
int			run_builtin(char ***envpc, char ***command_args_string,
				t_command_args ***command_args, int *fds);
int			run_in_parent(t_command_args ***command_args, int index,
				char ***envpc, char ***command_args_string);
void		run_in_child(t_command_args ***command_args, int index,
				char ***envpc, char ***command_args_string);
void		execute_in_child_signal(char p_or_c);
void		close_writefd_readfd(t_command_args **command_args, int i);
void		execute_in_child(t_command_args **command_args, int index,
				char ***envpc, char **cmd_args_str);
void		update_question_mark(char ***envpc, int status, int last_status);
void		handle_command_errors(int *status, int command_type,
				char **cmd_arg_str);
void		execution_close_fds(t_command_args ***command_args, int i);
void		wait_childs_update_status(t_command_args ***command_args,
				int *status);
void		execute_cmd(t_execution exe, int i);
t_execution	initialise_execution(t_command_args ***command_args, char ***envpc,
				int *status, int *command_type);
void		reset_status_command_type(int *status, int *command_type);
void		execution(t_command_args ***command_args, char ***envpc);
#endif