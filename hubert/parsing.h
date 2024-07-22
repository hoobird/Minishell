#ifndef PARSING_H
#define PARSING_H

typedef enum	e_tokentype
{
	word = 1,
	squote = 2,
	dquote = 3,
	re_output = 41,	// redirection >  be 41
	re_append = 42,	// redirection >> be 42
	re_input = 43,	// redirection <  be 43
	re_heredoc = 44	// redirection << be 44
}				t_tokentype;

typedef struct s_word_desc
{
	char *word; /* Zero terminated string. */
	int flags;	/* Flags associated with this word. */
}	t_word_desc;

typedef struct s_word_list
{
	struct s_word_list *next;
	t_word_desc *word;
}	t_word_list;

#endif