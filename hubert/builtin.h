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

# endif

