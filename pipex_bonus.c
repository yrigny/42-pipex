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

void	child_first(char *filename, t_proc *child, int pipe[2])
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
		perror(child->cmd_arr[0]);
		return ;
	}
	dup2(fd_src, STDIN_FILENO);
	dup2(pipe[1], STDOUT_FILENO);
	close(pipe[0]);
	execve(child->path, child->cmd_arr, NULL);
	perror("pipex");
}

void	child_middle(t_proc *child, int pipe1[2], int pipe2[2])
{
	if (!child->path)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return ;
	}
	if (access(child->path, X_OK) != 0)
	{
		perror(child->cmd_arr[0]);
		return ;
	}
	dup2(pipe1[0], STDIN_FILENO);
	dup2(pipe2[1], STDOUT_FILENO);
	close(pipe1[1]);
	close(pipe2[0]);
	execve(child->path, child->cmd_arr, NULL);
	perror("pipex");
}

void	child_last(char *filename, t_proc *child, int pipe[2])
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
		perror(child->cmd_arr[0]);
		return ;
	}
	dup2(pipe[0], STDIN_FILENO);
	dup2(fd_dst, STDOUT_FILENO);
	close(pipe[1]);
	execve(child->path, child->cmd_arr, NULL);
	perror("pipex");
}

void	fork_children(int ac, char **av, t_pipe book[MAX_PIPE], t_list *cmds)
{
	int		j;
	t_list	*head;

	j = -1;
	head = cmds;
	while (++j < ac - 3)
	{
		if (j > 1)
		{
			close(book[j - 2].fd[0]);
			close(book[j - 2].fd[1]);
		}
		book[j].pid = fork();
		if (book[j].pid == -1)
			free_exit(&cmds, EXIT_FAILURE);
		if (book[j].pid == 0 && j == 0)
			child_first(av[1], cmds->content, book[0].fd);
		else if (book[j].pid == 0 && j < ac - 4)
			child_middle(cmds->content, book[j - 1].fd, book[j].fd);
		else if (book[j].pid == 0 && j == ac - 4)
			child_last(av[ac - 1], cmds->content, book[j - 1].fd);
		if (book[j].pid == 0)
			free_exit(&head, EXIT_FAILURE);
		cmds = cmds->next;
	}
}

void	pipex(t_list *cmds, int ac, char **av)
{
	t_pipe	book[MAX_PIPE];
	int		i;
	t_list	*head;

	i = -1;
	head = cmds;
	while (++i < ac - 4)
	{
		if (pipe(book[i].fd) == -1)
			free_exit(&cmds, EXIT_FAILURE);
	}
	fork_children(ac, av, book, cmds);
	close(book[i - 1].fd[0]);
	close(book[i - 1].fd[1]);
	i = -1;
	while (++i < ac - 3)
		waitpid(book[i].pid, NULL, 0);
}
