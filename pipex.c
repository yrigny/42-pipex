/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:59:40 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/09 15:59:43 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_exit(t_proc (*child)[2], int status)
{
	int	i;

	i = -1;
	while ((*child)[0].cmd_arr[++i])
		free((*child)[0].cmd_arr[i]);
	free((*child)[0].cmd_arr);
	free((*child)[0].path);
	i = -1;
	while ((*child)[1].cmd_arr[++i])
		free((*child)[1].cmd_arr[i]);
	free((*child)[1].cmd_arr);
	free((*child)[1].path);
	exit(status);
}

void	child_one(char *filename, char **envp, t_proc child, int fd[2])
{
	int	fd_src;

	if (!child.path)
	{
		ft_putstr_fd(child.cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	if (access(child.path, X_OK) != 0)
	{
		perror("pipex");
		return ;
	}
	fd_src = open(filename, O_RDONLY, 0644);
	if (fd_src == -1)
	{
		perror("pipex");
		return ;
	}
	dup2(fd_src, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	execve(child.path, child.cmd_arr, envp);
	perror("pipex");
	exit(EXIT_FAILURE);
}

void	child_two(char *filename, char **envp, t_proc child, int fd[2])
{
	int	fd_dst;

	if (!child.path)
	{
		ft_putstr_fd(child.cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	if (access(child.path, X_OK) != 0)
	{
		perror("pipex");
		return ;
	}
	fd_dst = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd_dst, STDOUT_FILENO);
	close(fd[1]);
	execve(child.path, child.cmd_arr, envp);
	perror("pipex");
	exit(EXIT_FAILURE);
}

void	pipex(t_proc (*child)[2], char **av, char **envp)
{
	int	fd[2];
	int	pid[2];

	if (pipe(fd) == -1)
		free_exit(child, EXIT_FAILURE);
	pid[0] = fork();
	if (pid[0] == -1)
		free_exit(child, EXIT_FAILURE);
	if (pid[0] == 0)
	{
		child_one(av[1], envp, *child[0], fd);
		free_exit(child, EXIT_FAILURE);
	}
	pid[1] = fork();
	if (pid[1] == -1)
		free_exit(child, EXIT_FAILURE);
	if (pid[1] == 0)
	{
		child_two(av[4], envp, *child[1], fd);
		free_exit(child, EXIT_FAILURE);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
}

void	parser(char *arg, char **envp, t_proc *child)
{
	char	**paths;
	char	*cmd_path_tmp;
	int		i;

	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	paths = ft_split((*envp + 5), ':');
	child->cmd_arr = ft_split(arg, ' ');
	child->path = NULL;
	i = -1;
	while (!child->path && child->cmd_arr[0] && paths[++i])
	{
		cmd_path_tmp = ft_strjoin(paths[i], "/");
		child->path = ft_strjoin(cmd_path_tmp, child->cmd_arr[0]);
		if (access(child->path, F_OK) != 0)
		{
			free(child->path);
			child->path = NULL;
		}
		free(cmd_path_tmp);
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
}
