/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jburlama <Jburlama@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 20:54:06 by Jburlama          #+#    #+#             */
/*   Updated: 2024/05/26 20:55:25 by Jburlama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_signal(void)
{
	struct sigaction	sig_ign;
	struct sigaction	sig;

	ft_memset(&sig, 0, sizeof(sig));
	ft_memset(&sig_ign, 0, sizeof(sig_ign));
	sig_ign.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sig_ign, NULL);
	sig.sa_handler = signal_handler;
	sigaction(SIGINT, &sig, NULL);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	update_signals(void)
{
	struct sigaction	sig;

	ft_memset(&sig, 0, sizeof(sig));
	sig.sa_handler = signal_handler_update;
	sigaction(SIGQUIT, &sig, NULL);
	sigaction(SIGINT, &sig, NULL);
}

void	signal_handler_update(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	else if (sig == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
}

void	default_sig(void)
{
	struct sigaction	sig;

	ft_memset(&sig, 0, sizeof(sig));
	sig.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &sig, NULL);
	sigaction(SIGINT, &sig, NULL);
}
