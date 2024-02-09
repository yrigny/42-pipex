/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 16:37:47 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/09 16:38:26 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_proc	child[2];

	if (ac != 5)
	{
		ft_putstr_fd("Error: Bad arguments.\n", 2);
		ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
		exit(EXIT_FAILURE);
	}
	child[0].cmd_arr = ft_split(av[2], ' ');
	if (child[0].cmd_arr[0] && access(child[0].cmd_arr[0], F_OK) == 0)
		child[0].path = ft_strdup(child[0].cmd_arr[0]);
	else
		find_path(envp, &child[0]);
	child[1].cmd_arr = ft_split(av[3], ' ');
	if (child[1].cmd_arr[0] && access(child[1].cmd_arr[0], F_OK) == 0)
		child[1].path = ft_strdup(child[1].cmd_arr[0]);
	else
		find_path(envp, &child[1]);
	pipex(&child, av, envp);
	free_exit(&child, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
