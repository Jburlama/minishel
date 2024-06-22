/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbritto- <vbritto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:26:55 by vbritto-          #+#    #+#             */
/*   Updated: 2024/06/22 16:26:19 by vbritto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_content(t_token **tmp, t_token **keep, t_data *data)
{
	if (!(*tmp)->content)
	{
		if (*tmp == *keep)
		{
			if ((*tmp)->next)
				data->head = (*tmp)->next;
			else
				data->head = NULL;
			//free(data->head->content);
			free(*tmp);
			*tmp = data->head;
		}
		else
		{
			if (!(*tmp)->next)
				(*keep)->next = NULL;
			else
				(*keep)->next = (*tmp)->next;
			free(*tmp);
			*tmp = *keep;
		}
	}
}

char *get_env_name(char *content, char *name_expanded, size_t *until_dollar, t_data *data)
{
	char	*env_name;
	size_t		i;

	env_name = NULL;
	i = *until_dollar - 1;
	while ((content[*until_dollar] != '\0') && (content[*until_dollar] != ' ')
		&& (content[*until_dollar] != 34) && (content[*until_dollar] != 39))
	{
		(*until_dollar)++;
	}
	env_name = ft_calloc(*until_dollar - i, sizeof(char));
	if(!env_name)
		panic("calloc_fail", data);
	ft_strlcpy(env_name, content + i + 1, *until_dollar - i);
	name_expanded = getenv(env_name);
	free(env_name);
	return (name_expanded);
}

char	*expand(char *content, t_data *data, size_t *until_dollar)
{
	char	*name_expanded;
	char 	*tmp;
	size_t	i;

	name_expanded = NULL;
	i = *until_dollar - 1;
	if ((content[i + 1] == '\0') || (content[i + 1] == ' ')
		|| (content[i + 1] == 34) || (content[i + 1] == 39))
		return (content);
	name_expanded = get_env_name(content, name_expanded, until_dollar, data);
	if (!name_expanded && (content[*until_dollar]) == '\0')
	{
		free(content);
		return (NULL);
	}
	tmp = ft_calloc((ft_strlen(content) + ft_strlen(name_expanded) - (*until_dollar - i) + 1), sizeof(char));
	if(!tmp)
		panic("calloc_fail", data);
	ft_strlcpy(tmp, content, i + 1);
	if (name_expanded)
		ft_strlcpy(tmp + i, name_expanded, ft_strlen(name_expanded) + 1);
	if (content[*until_dollar] != '\0')
		ft_strlcpy(tmp + i + ft_strlen(name_expanded), content + (*until_dollar), ft_strlen(content) + 1 - (*until_dollar));
	free(content);
	return (tmp);
}

void	prepare_dollar(t_data *data)
{
	size_t		until_dollar;
	t_token *tmp;
	t_token	*keep;

	tmp = data->head;
	keep = tmp;
	while (tmp)
	{
		until_dollar = 0;
		while (tmp->content[until_dollar])
		{
			if (tmp->content[until_dollar] == '$' && tmp->type != SQUOTES)
			{
				until_dollar++;
				tmp->content = expand(tmp->content, data, &until_dollar);
				check_content(&tmp, &keep, data);
				until_dollar = 0;
			}
			until_dollar++;
		}
		keep = tmp;
		tmp = tmp->next;
	}
}
