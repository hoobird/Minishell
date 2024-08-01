# include "minishell.h"

int	check_envvar_delimiter(char *start)
{
	int	i;

	if (start[0] != '$')
		return (0);
	if (start[1] && !(ft_isalpha(start[1]) || start[1] == '_'))
		return (1);
	i = 2;
	while (start[i] && (ft_isalnum(start[i]) || start[i] == '_'))
		i++;
	return (i);
}

char	*shell_expand_append(char **envp, char **str, char *input, int len)
{
	char	*temp;
	char 	*key;
	char	*value;

	if (*input == '$')
	{
		key = ft_substr(input, 1, len - 1);
		value = envpc_get_value(envp, key);
		if (value)
			temp = ft_strjoin(*str, value);
		else
			temp = ft_strjoin(*str, "");
	}
	else
	{
		key = ft_substr(input, 0, len);
		temp = ft_strjoin(*str, key);
	}
	free(*str);
	free(key);
	return (temp);
}

char	*expandshellvar(char *input, char **envp)
{
	int		i;
	int		shellvar_len;
	char	*output;

	i=0;
	output = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$')
		{
			shellvar_len = check_envvar_delimiter(&input[i]);
			if (shellvar_len == 1)
				output = shell_expand_append(envp, &output, &input[i], 1);
			else
				output = shell_expand_append(envp, &output, &input[i], shellvar_len);
			i+= shellvar_len;
		}
		else
		{
			output = shell_expand_append(envp, &output, &input[i], 1);
			i++;
		}
	}
	return (output);
}

/*
// cc expand_shell_var2.c builtin_env.c ../Libft/libft.a
int	main(int argc, char **argv, char **env)
{
	char	*output;
	char 	**envpc;
	char	*input = "$HOME$HI $USER $HOME$USER$HOME";

	envpc = envp_copy(env);
	output = expandshellvar(input, envpc);
	printf("%s\n", output);
	free(output);
	envpc_free(&envpc);
	return (0);
}
*/
