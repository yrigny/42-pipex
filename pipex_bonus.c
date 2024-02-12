/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:59:40 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/11 18:42:12 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_exit(t_list **cmds, int status)
{
	t_list	*tmp;
	t_proc	*child;

	if (!cmds)
		return ;
	while (*cmds)
	{
		tmp = (*cmds)->next;
		child = (*cmds)->content;
		while (child->cmd_arr++)
			free(child->cmd_arr);
		free(child->cmd_arr);
		free(child->path);
		free(child);
		free(*cmds);
		*cmds = tmp;
	}
	exit(status);
}

void	child_first(char *filename, char **envp, t_proc *child, int pipe[2])
{
	int	fd_src;

	fd_src = open(filename, O_RDONLY, 0644);
	if (fd_src == -1)
	{
		perror(filename);
		return ;
	}
	if (!child->path)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	if (access(child->path, X_OK) != 0)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		perror("");
		return ;
	}
	dup2(fd_src, STDIN_FILENO);
	dup2(pipe[1], STDOUT_FILENO);
	close(pipe[0]);
	execve(child->path, child->cmd_arr, envp);
	perror("pipex");
}

void	child_middle(char **envp, t_proc * child, int pipe1[2], int pipe2[2])
{
	if (!child->path)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	if (access(child->path, X_OK) != 0)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		perror("");
		return ;
	}
	dup2(pipe1[0], STDIN_FILENO);
	dup2(pipe2[1], STDOUT_FILENO);
	close(pipe1[1]);
	close(pipe2[0]);
	execve(child->path, child->cmd_arr, envp);
	perror("pipex");
}

void	child_last(char *filename, char **envp, t_proc *child, int pipe[2])
{
	int	fd_dst;

	fd_dst = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!child->path)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	if (access(child->path, X_OK) != 0)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		perror("");
		return ;
	}
	dup2(pipe[0], STDIN_FILENO);
	dup2(fd_dst, STDOUT_FILENO);
	close(pipe[1]);
	execve(child->path, child->cmd_arr, envp);
	perror("pipex");
	// exit(EXIT_FAILURE);
}

void	pipex(t_list **cmds, int ac, char **av, char **envp)
{
	int	fd[ac - 4][2];
	int	pid[ac - 3];
	int	i = -1;
	int	j = -1;

	while (++i < ac - 4)
	{
		printf("pipe fd[%d]\n", i);
		if (pipe(fd[i]) == -1)
			free_exit(cmds, EXIT_FAILURE);
		printf("fd[%d][0] = %d\n", i, fd[i][0]);
		printf("fd[%d][1] = %d\n", i, fd[i][1]);
	}
	printf("i = %d\n", i);
	while (++j < ac - 3)
	{
		pid[j] = fork();
		if (pid[j] == -1)
			free_exit(cmds, EXIT_FAILURE);
		if (pid[j] == 0 && j == 0)
			child_first(av[1], envp, (*cmds)->content, fd[0]);
		if (pid[j] == 0 && j < ac - 4)
			child_middle(envp, (*cmds)->content, fd[j - 1], fd[j]);
		if (pid[j] == 0 && j == ac - 4)
			child_last(av[ac - 1], envp, (*cmds)->content, fd[ac - 4]);
		if (pid[j] == 0)
			free_exit(cmds, EXIT_FAILURE);
		*cmds = (*cmds)->next;
	}
	while (--i >= 0)
	{
		printf("close fd[%d][0]\nclose fd[%d][1]\n", i, i);
		close(fd[i][0]);
		close(fd[i][1]);
	}
	j = -1;
	while (++j < ac - 3)
	{
		printf("wait pid[%d]\n", j);
		waitpid(pid[j], NULL, 0);
	}
}
