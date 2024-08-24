/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:25:55 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:50:46 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

// builtin_env.c
int		builtin_export(char **args, char ***envpc);
int		builtin_unset(char **args, char ***envpc);
int		builtin_env(char **args, char ***envpc);

// builtin_env_helper.c
char	**envp_copy(char **envp);
int		envpc_length(char **envp);
char	*envpc_combine_key_value(char *key, char *value);
int		envpc_find_index(char **envpc, char *name);
char	*envpc_get_value(char **envpc, char *name);

// builtin_env_helper2.c
void	envpc_add(char ***envpc, char *name, char *value);
void	envpc_remove(char ***envpc, char *name);
void	envpc_free(char ***envpc);
void	envpc_print(char **envpc);
int		check_if_key_legit(char *key);

// builtin_env_helper3.c
void	freekeyvalue(char ***key_value);
void	add_to_env_and_free(char ***envpc, char **key, char **value);
int		try_add_envvar(char *kvpair, char ***envpc);

// expand_shell_var2.c
int		check_envvar_delimiter(char *start);
char	*shell_expand_append(char **envp, char **str, char *input, int len);
char	*expandshellvar(char *input, char **envp);

// builtin_cd_pwd.c
int		builtin_cd(char **arg, char ***envpc);
int		builtin_pwd(char **arg, char ***envpc);

// builtin_echo.c
int		builtin_echo(char **args);

// builtin_exit.c
void	builtin_exit_string(char ***status, char ***envpc,
			t_command_args ***command_args, int *fds);

// defined in main but needed in execution also hence its in bultins.c
void	freecommandlist(t_command_args ***command_args_list);

#endif
