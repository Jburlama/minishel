/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jburlama <Jburlama@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 19:02:06 by Jburlama          #+#    #+#             */
/*   Updated: 2024/05/31 16:18:49 by Jburlama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	tokenize(t_data *data)
{
	int	i;

	i = 0;
	while (rl_line_buffer[i] && rl_line_buffer[i] != '#')
	{
		if (is_quote(rl_line_buffer[i]))
			add_token(data, &i, QUOTES);
		else if (is_special(rl_line_buffer[i]))
			add_token(data, &i, SPECIAL);
		else if (ft_isprint(rl_line_buffer[i]) && rl_line_buffer[i] != 32)
		{
			if (data->head
				&& (data->tail->type == CMD || data->tail->type == QUOTES
					|| data->tail->type == I || data->tail->type == O))
				add_token(data, &i, ARG);
			else if (data->head
				&& (*data->tail->content == '<' || *data->tail->content == '>'))
				add_token(data, &i, IO);
			else
				add_token(data, &i, CMD);
		}
		i++;
	}
}

void	add_token(t_data *data, int *i, enum e_type type)
{
	if (type == SPECIAL)
		add_token_special(data, i, type);
	else if (type == CMD)
		add_token_word(data, i, type);
	else if (type == ARG)
		append_token_arg(data, i, type);
	else if (type == QUOTES)
		add_token_quotes(data, i, type);
	else if (type == IO)
	{
		if (*data->tail->content == '<')
			add_token_word(data, i, I);
		else if (*data->tail->content == '>')
			add_token_word(data, i, O);
	}
}

void	clear_list(t_token	**head)
{
	t_token	*temp;

	temp = *head;
	while (temp)
	{
		*head = (*head)->next;
		free(temp->content);
		free(temp);
		temp = *head;
	}
	*head = NULL;
}

bool	is_special(char c)
{
	char	*special;
	int		i;

	special = "|<>";
	i = 0;
	while (special[i])
	{
		if (c == special[i])
			return (true);
		i++;
	}
	return (false);
}

bool	is_quote(char c)
{
	if (c == 34 || c == 39)
		return (true);
	else
		return (false);
}
