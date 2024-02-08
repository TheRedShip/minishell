/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_auth.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 00:56:54 by marvin            #+#    #+#             */
/*   Updated: 2024/02/08 23:54:45 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_valid_token(t_token *t)
{
	t_token_type	bops;
	t_token_type	strs;

	bops = TK_BINOPS | TK_PIPEXS;
	strs = TK_STRING | TK_REDIRS;
	if (t->type & bops)
		return (0);
	while (t->next)
	{
		if ((t->type & TK_BINOPS) && (t->next->type & bops))
			return (0);
		else if (!ft_strncmp(t->str, "(", 2) && (t->next->type & bops))
			return (0);
		else if (!ft_strncmp(t->str, ")", 2) && (t->next->type & strs))
			return (0);
		else if (t->type == TK_REDIRS && !(t->next->type & TK_STRING))
			return (0);
		else if (t->type == TK_PIPEXS && (t->next->type & bops))
			return (0);
		t = t->next;
	}
	if (t->type & (TK_BRACES | TK_STRING))
		return (42);
	return (0);
}

int	ft_valid_braces(t_token *tk)
{
	int		isvalid;
	int		level;

	level = 0;
	isvalid = 0;
	if (!tk || !(tk->type & TK_BRACES && !ft_strncmp(tk->str, "(", 2)))
		return (1);
	tk = tk->next;
	while (tk && (!(tk->type & TK_BRACES && !ft_strncmp(tk->str, ")", 2)) || level))
	{
		if (tk->type & TK_BRACES && !ft_strncmp(tk->str, "(", 2))
			level++;
		if (tk->type & TK_BRACES && !ft_strncmp(tk->str, ")", 2))
			level--;
		if (!level && tk->type & (TK_BINOPS | TK_PIPEXS))
			isvalid = 1;
		tk = tk->next;
	}
	if (!tk)
		return (1);
	if (!(tk && isvalid))
		ft_remove_token(&tk);
	return (tk && isvalid);
}

void	ft_remove_braces(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (!ft_valid_braces(tmp))
		{
			ft_remove_token(&tmp);
			if (!tmp)
			{
				*tokens = NULL;
				continue ;
			}
			if (!tmp->prev)
				*tokens = tmp;
			continue;
		}
		tmp = tmp->next;
	}
}
