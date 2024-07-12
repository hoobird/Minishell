# include "minishell.h"

// error ive seen so far
// minishell: a: command not found
// minishell: syntax error near unexpected token `>'
// minishell: Command 'cata' not found
int	printerror(char	*errormsg)
{
	ft_putstr_fd("minishell: " ,2);
	ft_putstr_fd(errormsg, 2);
	return (0);
}