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
	parser(av[2], envp, &child[0]);
	parser(av[3], envp, &child[1]);
	pipex(&child, av, envp);
	free_exit(&child, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
