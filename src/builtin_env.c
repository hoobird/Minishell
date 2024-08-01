# include "minishell.h"

// copies the envp array of strings
// takes in original envp
// returns copied envp
char    **envp_copy(char **envp)
{
	int i;
	char **newenvp;

	newenvp = calloc(envpc_length(envp) + 1, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		newenvp[i] = ft_strdup(envp[i]);
		i++;
	}
	return (newenvp);
}

// returns the length of the envp array of strings
int	envpc_length(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

// combines key and value into a single string
char	*envpc_combine_key_value(char *key, char *value)
{
	char	*temp;
	char	*combined;

	temp = ft_strjoin(key, "=");
	if (value == NULL)
		value = "";
	combined = ft_strjoin(temp, value);
	free(temp);
	return (combined);
}

// finds the index of the key in the envpc array of strings
int	envpc_find_index(char **envpc, char *name)
{
	int	i;
	char	**temp;

	i=0;
	while (envpc[i])
	{
		temp = ft_split(envpc[i], '=');
		if (ft_strncmp(temp[0], name, ft_strlen(name) + 1) == 0)
		{
			envpc_free(&temp);
			return (i);
		}
		envpc_free(&temp);   // envpc_free also works cuz it technically frees the array of strings char**
		i++;
	}
	return (-1);
}

// gets the value of the key in the envpc array of strings
char	*envpc_get_value(char **envpc, char *name)
{
	int	i;
	char	**temp;
	char	*value;

	i = 0;
	while (envpc[i])
	{
		temp = ft_split(envpc[i], '=');
		if (ft_strncmp(temp[0], name, ft_strlen(name) + 1) == 0)
		{
			value = ft_strchr(envpc[i], '=') + 1;
			envpc_free(&temp);
			return (value);
		}
		envpc_free(&temp);
		i++;
	}
	return (NULL);
}

// adds a key value pair to the envpc array of strings
void	envpc_add(char ***envpc, char *name, char *value)
{
	char	**newenvpc;
	int		newlength;	// not including the NULL terminator
	int		i;
	char 	**temp;

	// if env key already exist, update the value
	if (envpc_find_index(*envpc, name) != -1)
	{
		temp = &(*envpc)[envpc_find_index((*envpc), name)];
		free(*temp);
		*temp = envpc_combine_key_value(name, value);
		return ;
	}
	// if env key does not exist, add new key value pair
	newlength = envpc_length((*envpc)) + 1;
	newenvpc = calloc(newlength + 1, sizeof(char *));
	newenvpc[newlength - 1] = envpc_combine_key_value(name, value);
	i = 0;
	while (i < newlength - 1)
	{
		newenvpc[i] = ft_strdup((*envpc)[i]);
		i++;
	}
	envpc_free(envpc);
	*envpc = newenvpc;
}

// removes the key from the envpc array of strings
void	envpc_remove(char ***envpc, char *name)
{
	char	**newenvpc;
	int		i;
	int		j;
	int		skip;

	skip = envpc_find_index(*envpc, name);
	if (skip == -1)
		return ;
	newenvpc = calloc(envpc_length((*envpc)) + 1, sizeof(char *));
	i = 0;
	j = 0;
	while ((*envpc)[i])
	{
		if (i == skip)
		{
			i++;
			continue;
		}
		newenvpc[j] = ft_strdup((*envpc)[i]);
		i++;
		j++;
	}
	envpc_free(envpc);
	*envpc = newenvpc;
}

// frees the envpc array of strings
void	envpc_free(char ***envpc)
{
	int	i;

	i = 0;
	while ((*envpc)[i])
	{
		free((*envpc)[i]);
		i++;
	}
	free(*envpc);
}

// prints the envpc array of strings
void	envpc_print(char **envpc)
{
	int	i;

	i = 0;
	while (envpc[i])
	{
		printf("%s\n", envpc[i]);
		i++;
	}
}

static int	args_length(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	check_if_key_legit(char *key)
{
	int	i;

	if (key[0] && !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i] && (ft_isalnum(key[i]) || key[i] == '_'))
		i++;
	return (i);
}

void	freekeyvalue(char ***key_value)
{
	free((*key_value)[0]);
	free((*key_value)[1]);
	free(*key_value);
}

int	try_add_envvar(char *kvpair, char ***envpc)
{
	char	**key_value;

	printf("kvpair: %s\n", kvpair);
	if (ft_strchr(kvpair, '=') == NULL)
	{
		printerror("export: `");
		ft_putstr_fd(kvpair, 2);
		ft_putstr_fd("': assignment operator (=) expected\n",2);
		return (1);
	}
	key_value = ft_split(kvpair, '=');
	if (check_if_key_legit(key_value[0]) == 0)
	{
		printerror("export: `");
		ft_putstr_fd(kvpair, 2);
		ft_putstr_fd("': not a valid identifier\n",2);
		freekeyvalue(&key_value);
		return (1);
	}
	envpc_add(envpc, key_value[0], key_value[1]);
	freekeyvalue(&key_value);
	return (0);
}

int	builtin_export(char **args, char ***envpc)
{
	int	i;
	int	exit;

	exit = 0;
	if (args_length(args) == 0)
	{
		printerror("export: at least 1 name assignment expected (eg: export example=abc123)\n");
		return (1);
	}
	i = 0;
	while (args[i])
	{
		if (try_add_envvar(args[i], envpc) == 1)
			exit = 1;
		i++;
	}
	return (exit);
}

int		builtin_unset(char **args, char ***envpc)
{
	int	i;
	int	exit;

	exit = 0;
	i = 0;
	while (args[i])
	{
		if (check_if_key_legit(args[i]) == 0)
			exit = 1;
		else
			envpc_remove(envpc, args[i]);
		i++;
	}
	return (exit);
}

int	builtin_env(char **args, char ***envpc)
{
	int	i;

	if (args_length(args) > 1)
	{
		printerror("env: no options or arguments expected\n");
		return (126);
	}
	i = 0;
	while ((*envpc)[i])
	{
		if (ft_strncmp((*envpc)[i], "?=", 2) != 0)
			printf("%s\n", (*envpc)[i]);
		i++;
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char	**envpc;

// 	envpc = envp_copy(envp);
// 	printf("Length: %d\n", envpc_length(envpc));
// 	envpc_add(&envpc, "test1", "fries");
// 	envpc_print(envpc);
// 	printf("Get value of test1: %s\n", envpc_get_value(envpc, "PWD"));
// 	envpc_add(&envpc, "PWD", "NASDKJHASDAS");
// 	printf("Get value of test1: %s\n", envpc_get_value(envpc, "PWD"));
// 	// envpc_add(&envpc, "test2", "chicken nuggets");
// 	// envpc_add(&envpc, "test3", "test");
// 	// printf("Search for test1: %s\n", envpc[envpc_find_index(envpc, "test1")]);
// 	// envpc_remove(&envpc, "test2");
// 	envpc_free(&envpc);
// 	return (0);
// }
