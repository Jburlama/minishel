/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbritto- <vbritto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:40:12 by vbritto-          #+#    #+#             */
/*   Updated: 2024/07/13 13:15:59 by vbritto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	execute(void *root, t_data *data)
{
	int	pid;
	int	wstatus;

	update_signals();
	pid = save_fork(data);
	wstatus = 0;
	if (pid == 0)
	{
		default_sig();
		if (root)
			runcmd(root, data);
		exit(errno);
	}
	waitpid(pid, &wstatus, 0);
}

void	runcmd(void *root, t_data *data)
{
	if (((t_exec *)root)->type == EXEC)
		runexec(root, data);
	else if (((t_exec *)root)->type == REDIR)
		runredir(root, data);
	else if (((t_pipe *)root)->type == PIPE)
		runpipe(root, data);
	else if (((t_cond *)root)->type == OR)
		runor(root, data);
	else if (((t_cond *)root)->type == AND)
		runand(root, data);
}

void	runredir(t_redir *root, t_data *data)
{
	int		fd;

	if (root->file_type == O)
	{
		fd = open(root->file, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (root->file_type == APEND)
	{
		fd = open(root->file, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (root->file_type == I)
		read_input(root, data);
	else if (root->file_type == HERE_DOC)
		here_doc(root, data);
	runcmd(((t_redir *)root)->down, data);
}

void	read_input(t_redir *root, t_data *data)
{
	int	fd;

	if (access(root->file, F_OK) == 0)
	{
		fd = open(root->file, O_RDWR);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		perror(root->file);
		clear_tree(data->root);
		exit(errno);
	}
}

void	runpipe(t_pipe *root, t_data *data)
{
	int	fds[2];
	int	wstatus;
	int	exit_code;

	pipe(fds);
	if (save_fork(data) == 0)
	{
		default_sig();
		dup2(fds[1], STDOUT_FILENO);
		close(fds[0]);
		close(fds[1]);
		runcmd(root->left, data);
	}
	if (save_fork(data) == 0)
	{
		default_sig();
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		close(fds[1]);
		runcmd(root->right, data);
	}
	close(fds[1]);
	close(fds[0]);
	while (waitpid(-1, &wstatus, 0) > 0)
	{
		if (WIFEXITED(wstatus))
			exit_code = WEXITSTATUS(wstatus);
	}
	clear_tree(data->root);
	exit(exit_code);
}
