/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 16:11:30 by rgramati          #+#    #+#             */
/*   Updated: 2024/02/15 18:49:52 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ft_quote_enforcer(char **str, int tmp_file_fd, t_quote_state qs)
{
	char			*string_holder;
	char			*dquote_holder;

	if (!*str)
		return ;
	ft_static_dq_holder(*str, NULL, 0, 0b00);
	string_holder = *str;
	while (**str)
		ft_qs_update(*((*str)++), &qs);
	if (qs)
	{
		dquote_holder = ft_open_dquote(tmp_file_fd, qs);
		if (!dquote_holder)
		{
			free(dquote_holder);
			*str = string_holder;
			return ;
		}
		dquote_holder = ft_strjoin(string_holder, dquote_holder, "\n", 0b01);
		ft_quote_enforcer(&dquote_holder, tmp_file_fd, QU_ZERO);
		*str = dquote_holder;
		return ;
	}
	*str = string_holder;
}

int	ft_quote_syntax(char *str, t_quote_state qs)
{
	while (*str)
		ft_qs_update(*(str++), &qs);
	return (qs);
}

void	ft_dequote_string(char **str, t_quote_state qs)
{
	char	*tmp;
	char	*tmpr;
	char	*res;

	tmp = *str;
	res = malloc((ft_dqstrlen(*str) + 1) * sizeof(char));
	if (!res)
		return ;
	tmpr = res;
	while (*tmp)
	{
		if (ft_qs_update(*tmp, &qs))
		{
			tmp++;
			continue ;
		}
		*(tmpr++) = *(tmp++);
	}
	*tmpr = 0;
	free(*str);
	*str = res;
}
