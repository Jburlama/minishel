/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jburlama <Jburlama@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:00:19 by Jburlama          #+#    #+#             */
/*   Updated: 2024/07/05 18:25:12 by Jburlama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define RED 		"\x1b[31m"
# define YELLOW 	"\x1b[33m"
# define GREEN 		"\x1b[32m"
# define BLUE 		"\x1b[34m"
# define LIGHT_GRAY "\x1b[37m"
# define DARK_GRAY  "\x1b[90m"
# define MAGENTA	"\033[1;35m"
# define RESET 		"\x1b[0m"

# include "libft/libft.h"
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>

extern int	status_exit;

enum e_type
{
	WHITE_SPACE,
	WORD,
	SPECIAL,
	DQUOTES,
	SQUOTES,
	IO,
	I,
	O,
	APEND,
	HERE_DOC,
	EXEC,
	REDIR,
	PIPE,
	AND,
	OR,
};

typedef struct s_token
{
	enum e_type		type;
	char			*content;
	struct s_token	*next;
}	t_token;

typedef struct s_cond
{
	enum e_type		type;
	void			*left;
	void			*right;
}	t_cond;

typedef struct s_pipe
{
	enum e_type		type;
	void			*left;
	void			*right;
}	t_pipe;

typedef struct s_redir
{
	enum e_type		type;
	enum e_type		file_type;
	char			*file;
	void			*down;
}	t_redir;

typedef struct s_exec
{
	enum e_type		type;
	char			**args;
}	t_exec;

typedef struct s_data
{
	t_token	*head;
	t_token	*tail;
	void	*root;
	char	**env;
}	t_data;

// execute.c
void	execute(t_data *data);
void	runcmd(void *root, t_data *data);
void	read_input(t_redir *root, t_data *data);
void	runpipe(t_pipe *root, t_data *data);
void	runredir(t_redir *root, t_data *data);

// here_doc.c
void	here_doc(t_redir *root);
char	*open_heredoc_for_write(int *fd);

// logical.c
void	runor(t_cond *root, t_data *data);
void	runand(t_cond *root, t_data *data);

// rumexrc.c
void	runexec(t_exec *node, t_data *data);
char	*get_pathname(char	*name, char **env);
char	**get_paths(char **env);

// create_tree.c
void	create_tree(t_data *data);

// construct.c
t_exec	*construct_exec(void);
void	*construct_redir(void	*subnode, t_token **tokens);
char	**add_to_args(char **args, char *content);
void	*construct_pipe(void *l, void *r);
void	*construct_cond(void *l, void *r, enum e_type type);

// parse_tree.c
void	*parse_exec(t_token **tokens);
void	*parse_block(t_token **tokens);
void	*parse_redir(void *root, t_token **tokens);
void	*parse_and(t_token **tokens);
void	*parse_or(t_token **tokens);
void	*parse_pipe(t_token **tokens);

// token_list_quotes.c
void	add_token_quotes(t_data *data, int *i, enum e_type type);
void	create_token_quotes(t_data *data, int *i, enum e_type type, char q);
int		create_token_quotes2(t_data *data, int *i, char q, int len);
void	append_token_quotes(t_data *data, int *i, enum e_type type, char q);

// token_list_quotes2.c
void	new_promp_for_quotes(t_data *data, char q, int len);
void	tokenize_continue(t_data *data, char q, int *i);
void	promp_quotes(char q);

// tokens_list_specil.c
void	add_token_special(t_data *data, int *i, enum e_type type);
void	create_token_special(t_data *data, int *i, enum e_type type);
void	append_token_special(t_data *data, int *i, enum e_type type);

// tokens_list_words.c
void	add_token_word(t_data *data, int *i, enum e_type type);
void	append_token_word(t_data *data, int *i, enum e_type type);
void	create_token_word(t_data *data, int *i, enum e_type type);
void	add_token_white_space(t_data *data, int *i, enum e_type type);

// tokenize.c
void	tokenize(t_data *data);
void	add_token(t_data *data, int *i, enum e_type type);
void	add_token_io(t_data *data, int *i);

// utils.c
pid_t	save_fork(t_data *data);
bool	is_all_white(char	*str);
void	jump_white_spaces(int *i);
bool	is_special(char c);
bool	is_quote(char c);

// signals.c
void	handle_signal(void);
void	signal_handler(int sig);
void	update_signals(void);
void	signal_handler_update(int sig);
void	default_sig(void);

// get_line.c
char	*get_line(void);
bool	is_white_space(char c);

// clear.c
void	clear_list(t_token	**head);
void	clear_tree(void	*root);
void	clear_args(char **args);
void	clear_gate(void	*root);

// panic.c
void	panic(char *msg, t_data *data);

// check.c
int		check(char *str);
void	check_redirect(char *str);
void	check_quotes(char *str);
void	ft_exit(char *str);
void	check_heredoc(char *str);

// prepare_token.c
void	prepare_token(t_data *data);
char	*expand(char *env_name, t_data *data);

#endif
