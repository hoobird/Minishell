#include "minishell.h"
#include "parsing.h"
#include "builtin.h"

// checks if its bash delimiter (|, spaces, \0, <, <<, >, >>)
// returns 0 if not delimiter, else number of spaces occpied by delimeter
int ft_isbashdelimiter(char *pt)
{
	if (ft_strncmp(pt, ">>", 2) == 0 || ft_strncmp(pt, "<<", 2) == 0)
		return (2);
	if (*pt == '|' || *pt == ' ' || *pt == '\t' || *pt == '<' || *pt == '>' || *pt == '$')
		return (1);
	return (0);
}

// return 1 if there is space, returns 0 if theres no space
int	check_if_space(char *str)
{
	if (*str == ' ' || *str == '\t')
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
void check_then_add_token(t_token **token, char *word, t_tokentype type, int spaces)
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
				*token = init_tokenlist(word, type, spaces);
			else
				add_token(*token, word, type, spaces);
			return ;
		}
		i++;
	}
	free(word); // free the word if its just made up of spaces
	// add_token(*token, ft_strdup(" "), WHITESPACE); // add space token
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
				if (i - start > 0) // if there is something before the quote
					check_then_add_token(&token, ft_substr(str, start, i - start), WORD, check_if_space(&str[i - 1]));
				quoted = str[i];
				start = i;
			}
			else
			{
				if (str[i] == '\'')
					check_then_add_token(&token, ft_substr(str, start+1, i - start-1), SQUOTE, check_if_space(&str[i+1]));
				else
					check_then_add_token(&token, ft_substr(str, start+1, i - start-1), DQUOTE, check_if_space(&str[i+1]));
				quoted = 0;
				start = i + 1;
			}
		}
		i++;
	}
	if (i - start > 0) // for the last word
	{
		if (quoted == 0)
			check_then_add_token(&token, ft_substr(str, start, i - start), WORD, check_if_space(&str[i - 1]));
		else
			check_then_add_token(&token, ft_substr(str, start, i - start), ERROR_UNCLOSED_QUOTES, 0);
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
					if (i - start > 0) // if there is something before the redirection
						check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD, check_if_space(&token->string[i - 1]));
					type = check_redirection_pipe_type(&(token->string[i]));
					if (type == RE_APPEND || type == RE_HEREDOC)
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 2), type, 0); // no need to check space
						i += 2;
					}
					else
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 1), type, 0); // no need to check space
						i++;
					}
					start = i;
				}
				else
					i++;
			}
			if (i - start > 0)
				check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD, token->postspace); // last token inherit the token's postspace
		}
		else
			check_then_add_token(&revisedtoken, ft_strdup(token->string), token->type, token->postspace);
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
				printerror("Syntax error near unexpected token `|'\n");
				return (1);
			}
		}
		else if (token->type == RE_OUTPUT || token->type == RE_APPEND || token->type == RE_INPUT || token->type == RE_HEREDOC)
		{
			next = token->next;
			if (next == NULL || next->type == PIPE || next->type == RE_OUTPUT || next->type == RE_APPEND || next->type == RE_INPUT || next->type == RE_HEREDOC)
			{
				free_tokenlist(&token);
				printerror("Syntax error near unexpected token `");
				ft_putstr_fd(token->string, 2);
				ft_putchar_fd('\n', 2);
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
			check_then_add_token(&newtoken, combined, token->type, nexttoken->postspace);
			token = nexttoken;
		}
		else
		{
			check_then_add_token(&newtoken, ft_strdup(token->string), token->type, token->postspace);
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
			check_then_add_token(&newtoken, expanded, token->type, token->postspace);
		}
		else
			check_then_add_token(&newtoken, ft_strdup(token->string), token->type, token->postspace);
		token = token->next;
	}
	return (newtoken);
}

// retokenise the WORD token after expansion
t_token	*retoken_word_after_expansion(t_token *etokens)
{
	t_token	*newtoken;
	char	**splitted;
	int		i;

	newtoken = NULL;
	while (etokens != NULL)
	{
		if (etokens->type == WORD)
		{
			splitted = ft_split(etokens->string, ' ');
			i = 0;
			while (splitted[i])
			{
				check_then_add_token(&newtoken, ft_strdup(splitted[i]), WORD, 1);
				free(splitted[i]);
				i++;
			}
			free(splitted);
			get_lasttoken(newtoken)->postspace = etokens->postspace;
		}
		else
			check_then_add_token(&newtoken, ft_strdup(etokens->string), etokens->type, etokens->postspace);
		etokens = etokens->next;
	}
	return (newtoken);
}

// cc parsing.c token_linkedlist.c printerror.c builtin_env.c expand_shell_var2.c ../Libft/libft.a -g
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
	ft_strlcpy(str, "cat file1.txt > \"hello.txt\" >> 'hi'muah | echo $USER \"$SHELL\" '$LANGUAGE'", 100);
	// ft_strlcpy(str, "  echo $USER  number2  \"    $USER\"      '$SHELL'$USER \"$SHELL\" 'sd' $PWD", 100);
	// ft_strlcpy(str, "", 100);
	
	printf("Command: ^%s\n", str);

	// STEP 1 - handle quotes
	tokens = process_quotes(str);
	free(str);
	// Step 1a - Error out when unclosed quotes
	if (check_error_process_quotes(tokens))
		return (1);
	// STEP 2  - handle special operators | > >> < <<
	revisedtokens = handle_redirection_pipe(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// Step 2a - check if redirection not next to each other
	// 			check if pipe is not next to pipe
	if (check_error_redirection_pipe(tokens))
		return (1);
	// STEP 3 - join redirects with file to the right
	revisedtokens = joinredirects(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 4 - handle shell vars
	revisedtokens = handle_shellvars(envpc, tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 5 - SPLIT WORD BY SPACES
	revisedtokens = retoken_word_after_expansion(tokens);
	free_tokenlist(&tokens);
	// STEP 6 - MERGE TOGETHER WORDS THAT ARE STUCK TOGETHER

	print_tokenlist(revisedtokens);
	free_tokenlist(&revisedtokens);
	return (0);
}