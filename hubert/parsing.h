#ifndef PARSING_H
#define PARSING_H

typedef enum	e_tokentype
{
	WORD = 1,
	SQUOTE = 2,
	DQUOTE = 3,
	RE_OUTPUT = 41,	// redirection >  be 41
	RE_APPEND = 42,	// redirection >> be 42
	RE_INPUT = 43,	// redirection <  be 43
	RE_HEREDOC = 44,	// redirection << be 44
	PIPE = 5,
	COMMAND = 6,
	ARGS = 7,
	// WHITESPACE = 6,
	ERROR_UNCLOSED_QUOTES = 90,
}				t_tokentype;

typedef struct s_token
{
	char 		*string; /* Zero terminated string. */
	t_tokentype	type;	/* Flags associated with this word. */
	int			postspace; /* 0 if no space after, 1 if got space after*/	
	struct s_token		*next;
}				t_token;

// token_linkedlist.c
t_token	*init_tokenlist(char *word, t_tokentype type, int spacesafter);
int		tokenlist_len(t_token *token);
t_token	*get_lasttoken(t_token *token);
void	add_token(t_token *token, char *word, t_tokentype type, int spacesafter);
void	add_token_after(t_token *token, char *word, t_tokentype type, int spacesafter);
int		get_tokenindex(t_token *token, t_token *target);
t_token	*get_token(t_token *token, int index);
int		search_token(t_token *token, char *word);
void	remove_token(t_token **token, int index);
void	free_tokenlist(t_token **token);
void	free_tokenlistlist(t_token ***tokenlist);
char	*tokenstostring(t_token *token);
void	print_tokenlist(t_token *token);


#endif
