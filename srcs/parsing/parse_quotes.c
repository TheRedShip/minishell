/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:53:31 by ycontre           #+#    #+#             */
/*   Updated: 2024/01/22 23:28:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_add(char *dest, char *src, size_t place)
{
	size_t	i;
	int		j;
	size_t	k;
	char *final_str;

	final_str = malloc(sizeof(char) * ft_strlen(dest) + ft_strlen(src) + 1);
	if (!final_str)
		return (NULL);
	final_str[ft_strlen(dest) + ft_strlen(src)] = '\0';
	i = -1;
	k = 0;
	while (++i < ft_strlen(dest) + ft_strlen(src))
	{
		if (i == place)
		{
			j = -1;
			while (src[++j])
				final_str[k++] = src[j];  
		}
		if (k < ft_strlen(dest) + ft_strlen(src))
			final_str[k] = dest[i];
		k++;
	}
	free(dest);
	return (final_str);
}

char	*str_append(char *str, char c)
{
	int		i;
    int 	len;
    char 	*new_str;

    len = ft_strlen(str);
    new_str = (char *)malloc(len + 2);
    if (!new_str)
        return (NULL);
    i = 0;
	while (str && str[i])
	{
		new_str[i] = str[i];
		i++;
	}
    new_str[len] = c;
    new_str[len + 1] = '\0';
	if (str != NULL)
    	free(str);
    return (new_str);
}

void	change_quote_state(int *quote_state, char *string, int *i)
{
	if (string[*i] == '"' && *quote_state == 0)
	{
		(*i)++;
		*quote_state = 1;
	}
	else if (string[*i] == '"' && *quote_state == 1)
	{
		(*i)++;
		*quote_state = 0;
		change_quote_state(quote_state, string, i);
	}
	else if (string[*i] == '\'' && *quote_state == 0)
	{
		(*i)++;
		*quote_state = 2;
	}
	else if (string[*i] == '\'' && *quote_state == 2)
	{
		(*i)++;
		*quote_state = 0;
		change_quote_state(quote_state, string, i);
	}
}

char	*get_env_var(char *string)
{
	int	i;
	int	j;
	char *env_var;

	i = 0;
	j = 0;
	while (ft_isalnum(string[i]))
		i++;
	env_var = (char *)malloc(sizeof(char) * (i + 1));
	if (!env_var)
		return (NULL);
	i = 0;
	while (ft_isalnum(string[i]))
	{
		env_var[j] = string[i];
		i++;
		j++;
	}
	env_var[j] = '\0';
	return (env_var);
}

char	*parse_quotes(char *string)
{
	int		i;
	int		j;
	int 	quote_status;
	char	*final_string;
	char	*env_var;

	i = -1;
	j = 0;
	quote_status = 0;
	final_string = NULL;
	while (i < (int)(ft_strlen(string)) && string[++i])
	{
		change_quote_state(&quote_status, string, &i);
		while (string[i] == '"' && quote_status == 1)
			change_quote_state(&quote_status, string, &i);
		while (string[i] == '\'' && quote_status == 2)
			change_quote_state(&quote_status, string, &i);
		if (string[i] == '$' && (ft_isalnum(string[i + 1])) && (quote_status == 0 || quote_status == 1))
		{
			env_var = get_env_var(string + i + 1);
			final_string = str_add(final_string, getenv(env_var), j);
			i += ft_strlen(env_var);
			j += ft_strlen(getenv(env_var));
			free(env_var);
		}
		else if (i < (int)(ft_strlen(string)))
		{
			final_string = str_append(final_string, string[i]);
			j++;
		}
	}
	free(string);
	return (final_string);	
}
