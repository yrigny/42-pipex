/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:11:34 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/11 18:42:22 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H

# define PIPEX_BONUS_H
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_proc
{
	char	**cmd_arr;
	char	*path;
}			t_proc;

void	free_exit(t_list **cmds, int status);
void	child_first(char *filename, char **envp, t_proc *child, int pipe[2]);
void	child_middle(char **envp, t_proc * child, int pipe1[2], int pipe2[2]);
void	child_last(char *filename, char **envp, t_proc *child, int pipe[2]);
void	pipex(t_list **cmds, int ac, char **av, char **envp);
void	find_path(char **paths, t_proc *child);
void	parse_cmds(int ac, char **av, char **paths, t_list **cmds);

#endif
