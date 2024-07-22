#include "minishell.h"
#include "parsing.h"


void	flipquoted(int	*quoted, char quotetype)
{
	if (*quoted > 0)
		*quoted = 0;
	else if (quotetype == '\'')
		*quoted = 1;
	else if (quotetype == '\"')
		*quoted = 2;
}

// checks if its bash delimiter (|, spaces, \0, <, <<, >, >>)
// returns 0 if not delimiter, else number of spaces occpied by delimeter
int ft_isbashdelimiter(char *pt)
{
	if (ft_strncmp(pt, ">>", 2) == 0 || ft_strncmp(pt, "<<", 2) == 0)
		return (2);
	if (*pt == '|' || *pt == 32 || *pt == '\t' || *pt == '<' || *pt == '>' || *pt == '$')
		return (1);
	return (0);
}

// split the string into tokens normal, single_quote, double_quote
t_word_list	process_quotes(char *str)
{
	// as we traverse the string, we will keep track of whether we are inside a quote or not
	int		quoted;
	int		i;

	quoted = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			flipquoted(&quoted, str[i]);
		else
		{
			
		}
		i++;
	}
		
}