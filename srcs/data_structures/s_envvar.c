/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_envvar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 22:33:30 by rgramati          #+#    #+#             */
/*   Updated: 2024/01/24 18:48:54 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envvar    *ft_init_var(char *value)
{
	t_envvar    *new_var;
	char        *name;

	new_var = malloc(sizeof(t_envvar));
	name = ft_strdup(ft_strtok(value, "="));
	new_var->name = name;
	new_var->values = ft_split(value + ft_strlen(name) + 1, ':');
	new_var->next = NULL;
	return (new_var);
}

void    ft_add_var(t_envvar **vars, t_envvar *var)
{
	t_envvar	*tmp;

	if (!vars || !var)
		return ;
	if (!(*vars))
	{
		*vars = var;
		return ;
	}
	tmp = *vars;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = var;
}

void	ft_remove_var(t_envvar **vars, char *name)
{
	t_envvar	*prv;
	t_envvar	*tmp;

	prv = NULL;
	tmp = *vars;
	while (ft_strcmp(tmp->name, name))
	{
		prv = tmp;
		tmp = tmp->next;
	}
	prv->next = tmp->next;
	ft_del_var(tmp);
}

void	ft_del_var(t_envvar *var)
{
	char	**tmp;

	tmp = var->values;
	free(var->name);
	while (*(var->values))
	{
		free(*(var->values));
		var->values++;
	}
	free(tmp);
}

t_envvar	*ft_get_var(t_envvar *vars, char *name)
{
	while (ft_strncmp(vars->name, name, ft_strlen(name)))
		vars = vars->next;
	return (vars);
}

void	ft_set_var(t_envvar *vars, char *name, char *nv)
{
	while (ft_strncmp(vars->name, name, ft_strlen(name)))
		vars = vars->next;
	ft_free_tab((void **)vars->values);
	vars->values = ft_split(nv, ':');
}