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

typedef struct s_arg
{
	char	**paths;
	char	*cmd1_path;
	char	*cmd2_path;
	char	**cmd1_arr;
	char	**cmd2_arr;
}			t_arg;

int			parser(int ac, char **av, char **envp, t_arg *ar);
void        find_fullpath(t_arg *ar, char *cmd, char **fullpath);
void		child_two(t_arg ar, char **av, char **envp, int fd[2]);
void		child_one(t_arg ar, char **av, char **envp, int fd[2]);
void		ar_init(t_arg *ar);
void		ar_free(t_arg *ar);
void		delete_free(char **ptr);
void		free_exit(t_arg *ar);

#endif