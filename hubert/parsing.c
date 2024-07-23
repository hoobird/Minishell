#include "minishell.h"
#include "parsing.h"
#include "builtin.h"

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

// check for redirections and pipes
t_tokentype	check_redirection_pipe_type(char *s)
{
	if (ft_strncmp(s, ">>", 2) == 0)
		return (RE_APPEND);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (RE_HEREDOC);
	if (ft_strncmp(s, ">", 1) == 0)
		return (RE_OUTPUT);
	if (ft_strncmp(s, "<",1)== 0)
		return (RE_INPUT);
	if (ft_strncmp(s, "|", 1) == 0)
		return (PIPE);
	return (0);
}

// add token with check if its empty string or just made up of spaces
// also check if token list is empty
void check_then_add_token(t_token **token, char *word, t_tokentype type)
{
	int 		i;

	if (ft_strlen(word) == 0) // check if string is empty, cuz dont want to add empty string
	{
		free(word);
		return ;
	}
	i = 0;
	while (word[i])
	{
		if (word[i] != ' ' && word[i] != '\t')
		{
			if (*token == NULL)
				*token = init_tokenlist(word, type);
			else
				add_token(*token, word, type);
			return ;
		}
		i++;
	}
	free(word); // free the word if its just made up of spaces
	add_token(*token, ft_strdup(" "), WHITESPACE); // add space token
}
	
// split the string into tokens normal, single_quote, double_quote
t_token	*process_quotes(char *str)
{
	// as we traverse the string, we will keep track of whether we are inside a quote or not
	char		quoted;
	int		i;
	int 	start;
	t_token	*token;

	quoted = 0;
	i = 0;
	start = 0;
	token = NULL;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (quoted == 0)
			{
				// add whats before the quote
				if (i - start > 0)
					check_then_add_token(&token, ft_substr(str, start, i - start), WORD);
				quoted = str[i];
				start = i;
			}
			else
			{
				if (str[i] == '\'')
					check_then_add_token(&token, ft_substr(str, start+1, i - start-1), SQUOTE);
				else
					check_then_add_token(&token, ft_substr(str, start+1, i - start-1), DQUOTE);
				quoted = 0;
				start = i + 1;
			}
		}
		i++;
	}
	if (i - start > 0)
	{
		if (quoted == 0)
			check_then_add_token(&token, ft_substr(str, start, i - start), WORD);
		else
			check_then_add_token(&token, ft_substr(str, start, i - start), ERROR_UNCLOSED_QUOTES);
	}
	return (token);
}

int		check_error_process_quotes(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == ERROR_UNCLOSED_QUOTES)
		{
			printerror("Unclosed quotes detected");
			free_tokenlist(&token);
			return (1);
		}
		token = token->next;
	}
	return (0);
}

// handle redirections and pipe chracter
t_token	*handle_redirection_pipe(t_token *token)
{
	int		i;
	int		start;
	t_token	*revisedtoken;
	t_tokentype type;

	revisedtoken = NULL;
	while (token != NULL)
	{
		if (token->type == WORD)
		{
			i = 0;
			start = 0;
			while (token->string[i])
			{
				if (check_redirection_pipe_type(&(token->string[i])))
				{
					if (i - start > 0)
						check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD);
					type = check_redirection_pipe_type(&(token->string[i]));
					if (type == RE_APPEND || type == RE_HEREDOC)
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 2), type);
						i += 2;
					}
					else
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 1), type);
						i++;
					}
					start = i;
				}
				else
					i++;
			}
			if (i - start > 0)
				check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD);
		}
		else
			check_then_add_token(&revisedtoken, ft_strdup(token->string), token->type);
		token = token->next;
	}
	return (revisedtoken);
}

// check if redirection not next to each other
// check if pipe is not next to pipe
int	check_error_redirection_pipe(t_token *token)
{
	t_token	*next;

	while (token != NULL)
	{
		if (token->type == PIPE)
		{
			next = token->next;
			if (next == NULL || next->type == PIPE)
			{
				free_tokenlist(&token);
				printerror("Syntax error near unexpected token `|'");
				return (1);
			}
		}
		else if (token->type == RE_OUTPUT || token->type == RE_APPEND || token->type == RE_INPUT || token->type == RE_HEREDOC)
		{
			next = token->next;
			if (next == NULL || next->type == PIPE || next->type == RE_OUTPUT || next->type == RE_APPEND || next->type == RE_INPUT || next->type == RE_HEREDOC)
			{
				free_tokenlist(&token);
				printerror("Syntax error near unexpected token `>'");
				return (1);
			}
		}
		token = token->next;
	}
	return (0);
}

//join redirects with file to the right
t_token	*joinredirects(t_token *token)
{
	t_token	*newtoken;
	t_token	*temp;
	t_token	*nexttoken;
	char	*combined;

	newtoken = NULL;
	while (token != NULL)
	{
		if (token->type == RE_OUTPUT || token->type == RE_APPEND || token->type == RE_INPUT || token->type == RE_HEREDOC)
		{
			nexttoken = token->next;
			combined = ft_strjoin(token->string, nexttoken->string);
			check_then_add_token(&newtoken, combined, token->type);
			token = nexttoken;
		}
		else
		{
			check_then_add_token(&newtoken, ft_strdup(token->string), token->type);
		}
		token = token->next;
	}
	return (newtoken);
}

t_token	*handle_shellvars(char **envp, t_token *token)
{
	t_token	*newtoken;
	t_token	*temp;
	char	*expanded;

	newtoken = NULL;
	while (token != NULL)
	{
		if (token->type == WORD || token->type == DQUOTE)
		{
			expanded = expandshellvar(token->string, envp);
			check_then_add_token(&newtoken, expanded, token->type);
		}
		else
			check_then_add_token(&newtoken, ft_strdup(token->string), token->type);
		token = token->next;
	}
	return (newtoken);
}

// cc parsing.c token_linkedlist.c printerror.c builtin_env.c ../Libft/libft.a -g
int main(int argc, char **argv, char **envp)
{
	t_token *tokens;
	t_token *revisedtokens;
	char	*str;
	char	**envpc;

	envpc = envp_copy(envp);
	str = ft_calloc(100, sizeof(char));
	// ft_strlcpy(str, "echo 'hello'  $USER   123\"asd\" | cat >file2 >> file3", 100);
	// ft_strlcpy(str, "echo 'hello world'    file.txt | \"adsasd\"boss'a'", 100);
	// ft_strlcpy(str, "cat file1.txt > \"hello.txt\" >> 'hi' | wc -l", 100);
	ft_strlcpy(str, "  echo $USER    \"    $USER\"      '$SHELL'$USER\"$SHELL\" 'sd' $PWD", 100);
	// ft_strlcpy(str, "", 100);
	
	// STEP 1 - handle quotes
	tokens = process_quotes(str);
	free(str);
	if (check_error_process_quotes(tokens))
		return (1);
	// STEP 2  - handle special operators | > >> < <<
	revisedtokens = handle_redirection_pipe(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	if (check_error_redirection_pipe(tokens))
		return (1);
	// STEP 3 - join redirects with file to the right
	revisedtokens = joinredirects(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 4 - handle shell vars
	revisedtokens = handle_shellvars(envpc, tokens);
	free_tokenlist(&tokens);
	print_tokenlist(revisedtokens);
	free_tokenlist(&revisedtokens);
	return (0);
}
