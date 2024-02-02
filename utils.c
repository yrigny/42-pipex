/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:26:01 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/02 16:26:03 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ar_init(t_arg *ar)
{
	ar->paths = NULL;
	ar->cmd1_path = NULL;
	ar->cmd2_path = NULL;
	ar->cmd1_arr = NULL;
	ar->cmd2_arr = NULL;
}

void	ar_free(t_arg *ar)
{
	int	i;

	i = -1;
	while (ar->paths[++i])
		free(ar->paths[i]);
	if (ar->paths)
		free(ar->paths);
	if (ar->cmd1_path)
		free(ar->cmd1_path);
	if (ar->cmd2_path)
		free(ar->cmd2_path);
	i = -1;
	while (ar->cmd1_arr[++i])
		free(ar->cmd1_arr[i]);
	if (ar->cmd1_arr)
		free(ar->cmd1_arr);
	i = -1;
	while (ar->cmd2_arr[++i])
		free(ar->cmd2_arr[i]);
	if (ar->cmd2_arr)
		free(ar->cmd2_arr);
}

void	delete_free(char **ptr)
{
	if (*ptr != NULL)
		free(*ptr);
	*ptr = NULL;
}

void	free_exit(t_arg *ar)
{
	ar_free(ar);
	exit(1);
}
