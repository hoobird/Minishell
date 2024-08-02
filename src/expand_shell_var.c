# include "minishell.h"

int	check_envvar_delimiter(char *start)
{
	int	i;

	// check if the first character is a dollar sign
	if (*start != '$')
		return (0);
	// check if its specificall $?
	if (start[1] == '?')
		return (2);
	// check if the next character is a letter or an underscore
	if (!ft_isalpha(start[1]) && start[1] != '_')
		return (1);
	// check if the following characters are a letter, an underscore, or a number
	i = 1;
	while (ft_isalnum(start[i]) || start[i] == '_')
		i++;
	return (i);
}

char	*shell_expand_append(char **envp, char **str, char *input, int len)
{
	char	*temp;
	char 	*key;
	char	*value;

	if (*input == '$' && len > 1)
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
