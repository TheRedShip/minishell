/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:01:13 by ycontre           #+#    #+#             */
/*   Updated: 2024/01/30 15:39:53 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**str_copy(char **string)
{
	char	**new_string;
	int		i;

	i = 0;
	while (string[i])
		i++;
	new_string = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (string[i])
	{
		new_string[i] = ft_strdup(string[i]);
		i++;
	}
	new_string[i] = NULL;
	return (new_string);
}

void	ft_print_logo(void)
{
	int		fd;
	char	*line;

	fd = open("logosang", O_RDONLY);
	if (fd < 0)
		return ;
	line = get_next_line(fd);
	printf("\033c\033[31m");
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	printf("\033[0m");
	printf("\033[37;1;3mby rgramati, ycontre\033[0m\n\n");
	close(fd);
}

int	main(int argc, char **argv, char **envp)
{
	t_envvar	*env;
	int 		debugging;

	(void) argc;
	(void) argv;
	debugging = -1;
	if (argc == 2 && !ft_strncmp(argv[1], "debug", 6))
	{
		debugging = open("debug", O_CREAT | O_TRUNC | O_WRONLY, 0644);
		dup2(debugging, STDOUT_FILENO);
	}
	toggle_signal(1);
	env = ft_setup_env(argv, envp);
	ft_update_env(&env);
	ft_print_logo();
	while (1)
		ft_prompt(&env);
	if (debugging < 0)
		close(debugging);
	return (0);
}
