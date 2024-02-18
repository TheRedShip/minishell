/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_command.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:00:02 by rgramati          #+#    #+#             */
/*   Updated: 2024/02/17 16:29:33 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_path(char *cmd, t_envvar *envp)
{
	int		i;
	char	*path;
	char	*bin;

	if (!access(cmd, F_OK))
		return (ft_strdup(cmd));
	while (envp && ft_strncmp(envp->name, "PATH", 5))
		envp = envp->next;
	if (!envp)
		return (NULL);
	bin = ft_strjoin("/", cmd, 0, 0b00);
	i = 0;
	path = ft_strjoin(envp->values[i++], bin, 0, 0);
	while (envp->values[i] && access(path, X_OK) == -1)
	{
		free(path);
		path = ft_strjoin(envp->values[i++], bin, 0, 0);
	}
	free(bin);
	return (path);
}

void	ft_del_command(t_command *cmd)
{
	if (cmd->infile > 2)
		close(cmd->infile);
	if (cmd->outfile > 2)
		close(cmd->outfile);
	free(cmd->path);
	ft_free_tab((void **)cmd->args);
	free(cmd);
}

void	ft_display_command(t_command *cmd)
{
	char	**tmp;

	tmp = NULL;
	printf("- COMMAND --- %p\n", cmd);
	printf("INFILE FD : %d\n", cmd->infile);
	printf("OUTFILE FD : %d\n", cmd->outfile);
	if (cmd->path)
		printf("PATH : %s\n", cmd->path);
	if (cmd->args)
	{
		tmp = cmd->args;
		printf("ARGS :\n");
		while (*tmp)
			printf("  val : %s\n", *(tmp++));
	}
	printf("ENV    : %p\n", cmd->envp);
	printf("--------------------------------------\n");
}
