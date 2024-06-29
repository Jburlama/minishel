/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbritto- <vbritto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:49:21 by vbritto-          #+#    #+#             */
/*   Updated: 2024/06/28 16:10:14 by vbritto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	execute_builtins(t_exec *node, t_data *data)
{
	if (node->builtin == ECHO)
		cmd_echo(data, node);
	/*if (node->builtin == CD)
		cmd_cd(node);
	if (node->builtin == PWD)
		cmd_pwd(node);*/
	if (node->builtin == EXPORT)
		cmd_export(data, node);
	if (node->builtin == UNSET)
		cmd_unset(data, node);
	if (node->builtin == ENV)
		cmd_env(data);
	//if (node->builtin == EXIT)
	//	cmd_exit(node);
	//clear_tree(data->root);
	//exit(errno);
}

int	pipe_and_builtin(t_data *data)
{
    void  *root;

    root = data->root;
    if (((t_pipe *)root)->type == PIPE && (data->head->builtin <= 2))
        return (0) ;
    else
	{
        execute_builtins(data->root, data);
		return (1);
	}
}