#ifndef PARSING_H
#define PARSING_H

#include "typedefs.h"

// parsing.c
int 		ft_isbashdelimiter(char *pt);
int			check_if_space(char *str);
t_tokentype	check_redirection_pipe_type(char *s);
void		check_then_add_token(t_token **token, char *word, t_tokentype type, int spaces);
t_token		*process_quotes(char *str);
void		process_quotes_handle_in_out_quote(t_token **token, char *str, char *quoted, int *start, int i);
void		process_quotes_add_last_word(t_token **token, char *str, int start, int i, int quoted);
int			check_error_process_quotes(t_token *token);
t_token		*handle_redirection_pipe(t_token *token);
int			check_error_redirection_pipe(t_token *token);
t_token		*joinredirects(t_token *token);
t_token		*handle_shellvars(char **envp, t_token *token);
t_token		*retoken_word_after_expansion(t_token *etokens);
t_token		*merge_stucktogether_words(t_token *etokens);
int			count_pipes(t_token *token);
t_token		**split_by_pipe(t_token *token);
void		label_commands_args(t_token **tokenlistlist);
t_token		**parse_input(char *str, char ***envp);
t_token		**parse_input_helper(t_token *tokens, char ***envp);

// token_linkedlist.c
t_token	*init_tokenlist(char *word, t_tokentype type, int spacesafter);
int		tokenlist_len(t_token *token);
int		tokenlistlist_len(t_token **tokenlist);
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
void	print_tokenlistlist(t_token **tokenlist);
int		check_tokenlistlist_empty_and_free(t_token **tokenlist);

#endif
