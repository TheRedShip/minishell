/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:02:28 by rgramati          #+#    #+#             */
/*   Updated: 2024/02/20 22:03:42 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error_code	ft_open_heredocs(t_node *tree, t_node *root, int *not_failed)
{
	t_error_code	err;
	t_redir			*tmp;
	char			*hd_file;

	err = ERR_NOERRS;
	if (!*not_failed)
		return (ERR_ERRORS);
	if (!tree->command)
	{
		err |= !!ft_open_heredocs(tree->left, root, not_failed);
		err |= !!ft_open_heredocs(tree->right, root, not_failed);
		return (err);
	}
	tmp = tree->command->redirs;
	while (tmp && tree->command->infile != -1 && *not_failed)
	{
		if (tmp->type == RD_HEREDOC)
		{
			hd_file = ft_get_temp_file(".heredoc", 16);
			if (tree->command->infile != STDIN_FILENO)
				close(tree->command->infile);
			tree->command->infile = \
			ft_get_heredoc(ft_strdup(tmp->file), hd_file, root);
			*not_failed = (tree->command->infile != OP_HDOCKO);
		}
		tmp = tmp->next;
	}
	return (tree->command->infile == -1 && *not_failed);
}

void	ft_open_file(t_command *cmd, char *file, int mode)
{
	int	*fd;

	if (mode == OPEN_READ)
		fd = &(cmd->infile);		
	else
		fd = &(cmd->outfile);
	if (*fd > 2)
		close(*fd);
	if (mode != OPEN_READ && *fd != -1)
		*fd = open(file, mode, 0644);
	else if (*fd != -1)
		*fd = open(file, mode);
	if (*fd == -1)
		ft_error_message(ERR_NOFORD, file);
	free(file);
}

t_error_code	ft_open_outputs(t_node *tree)
{
	t_redir			*tmp;
	t_error_code	err;

	err = ERR_NOERRS;
	if (!tree->command)
	{
		err |= ft_open_outputs(tree->left);
		err |= ft_open_outputs(tree->right);
		return (err);
	}
	tmp = tree->command->redirs;
	while (tmp && tree->command->outfile != -1)
	{
		if (tmp->type == RD_INFILES && access(tmp->file, R_OK))
			break ;
		if (tmp->type == RD_OUTPUTS)
			ft_open_file(tree->command, ft_strdup(tmp->file), OPEN_CREATE);
		else if (tmp->type == RD_APPENDS)
			ft_open_file(tree->command, ft_strdup(tmp->file), OPEN_APPEND);
		tmp = tmp->next;
	}
	return (tree->command->outfile == -1 && access(tmp->file, R_OK));
}

t_error_code	ft_open_inputs(t_node *tree)
{
	t_redir			*tmp;

	if (!tree->command->redirs)
		return (ERR_NOERRS);
	tmp = tree->command->redirs;
	while (tmp && tree->command->infile != -1)
	{
		if (tmp->type == RD_INFILES)
			ft_open_file(tree->command, ft_strdup(tmp->file), OPEN_READ);
		tmp = tmp->next;
	}
	return (tree->command->infile == -1);
}


//ON VA CHANGER BCP DE CHOSES ICI !