# ifndef BUILTIN_H
# define BUILTIN_H

// builtin_env.c
char    **envp_copy(char **envp);
void	envpc_print(char **envpc);
int		envpc_length(char **envp);
void	envpc_add(char ***envpc, char *name, char *value);
void	envpc_remove(char ***envpc, char *name);
void	envpc_free(char ***envpc);
char	*envpc_combine_key_value(char *key, char *value);
int		envpc_find_index(char **envpc, char *name);
char	*envpc_get_value(char **envpc, char *name);

// expand_shell_var2.c
int		check_envvar_delimiter(char *start);
char	*shell_expand_append(char **envp, char **str, char *input, int len);
char	*expandshellvar(char *input, char **envp);

// builtin_cd_pwd.c
void	builtin_cd(char **arg, char ***envpc);
void	builtin_pwd(char **arg, char ***envpc);

// builtin_echo.c
void	builtin_echo(char *flag, char **arg);
int		check_only_n(char *flag);

# endif

