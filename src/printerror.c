# include "minishell.h"

// error ive seen so far
// minishell: a: command not found
// minishell: syntax error near unexpected token `>'
// minishell: Command 'cata' not found
int	printerror(char	*errormsg)
{
	char	*temp;

	temp = ft_strjoin("minishell: ", errormsg);
	ft_putstr_fd(temp, 2);
	free(temp);
	return (1);
}