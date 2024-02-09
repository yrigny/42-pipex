/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:11:34 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/26 14:43:04 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H
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

void		find_path(char **envp, t_proc *child);
void		pipex(t_proc (*child)[2], char **av, char **envp);
void		child_one(char *filename, char **envp, t_proc child, int fd[2]);
void		child_two(char *filename, char **envp, t_proc child, int fd[2]);
void		free_exit(t_proc (*child)[2], int status);

#endif