# include "minishell.h"
# include "builtin.h"

// cc expand_shell_var2.c builtin_env.c ../Libft/libft.a

char	*addquotes(char* input, int	type) // type: 1 = single quote, 2 = double quote
{
	char	*output;

	output = ft_calloc(ft_strlen(input) + 3, sizeof(char));
	if (type == 1)
	{
		output[0] = '\'';
		ft_strlcpy(&output[1], input, ft_strlen(input) + 1);
		output[ft_strlen(input) + 1] = '\'';
	}
	else if (type == 2)
	{
		output[0] = '"';
		ft_strlcpy(&output[1], input, ft_strlen(input) + 1);
		output[ft_strlen(input) + 1] = '"';
	}
	free(input);
	return (output);
}	

int	checkshelldelimiter(char *start)
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

char	*expandshellvar(char *input, char **env)
{
	int		i;
	int		quoted;
	int		shellvarlen;
	char	*front;
	char 	*back;

	quoted = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
		{
			if (quoted == 1)
				quoted = 0;
			else
				quoted = 1;
		}
		else if (input[i] == '"')
		{
			if (quoted == 2)
				quoted = 0;
			else
				quoted = 2;
		}
		if (input[i] == '$' && quoted != 1)
		{
			shellvarlen = checkshelldelimiter(&input[i]);
			if (shellvarlen = 1)
			{
				front = ft_substr(input, 0, i);
				back = ft_strdup(&input[i + 1]);
				free(input);
				input = ft_strjoin(front, back);
				free(front);
				free(back);
			}
		}

		i++;
	}
	return NULL;
}


int	main(int argc, char **argv, char **env)
{
	char	*input = "$$HOME $HI";
	printf("len = %d\n", checkshelldelimiter(input));
	return (0);
}