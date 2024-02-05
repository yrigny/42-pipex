/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 18:07:36 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/26 18:09:33 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_arg ar, char **av, char **envp, int fd[2])
{
	int	exec_status;
	int	fd_src;

	fd_src = open(av[1], O_RDONLY, 0644);
	if (fd_src == -1)
	{
		ft_printf("%s: %s\n", strerror(errno), av[1]);
		return ;
	}
	dup2(fd_src, STDIN_FILENO);
	close(fd_src);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	exec_status = execve(ar.cmd1_path, ar.cmd1_arr, envp);
	if (exec_status == -1)
		perror("");
}

void	child_two(t_arg ar, char **av, char **envp, int fd[2])
{
	int	exec_status;
	int	fd_dst;

	fd_dst = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_dst == -1)
	{
		ft_printf("%s: %s\n", strerror(errno), av[4]);
		return ;
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	dup2(fd_dst, STDOUT_FILENO);
	close(fd_dst);
	exec_status = execve(ar.cmd2_path, ar.cmd2_arr, envp);
	if (exec_status == -1)
		perror("");
}

void	find_fullpath(t_arg *ar, char *cmd, char **fullpath)
{
	char	*cmd_path_tmp;
	int		i;

	// cmd_path_tmp = NULL;
	i = -1;
	while (!(*fullpath) && cmd && ar->paths[++i])
	{
		cmd_path_tmp = ft_strjoin(ar->paths[i], "/");
		*fullpath = ft_strjoin(cmd_path_tmp, cmd);
		if (access(*fullpath, F_OK) != 0)
			delete_free(fullpath);
		free(cmd_path_tmp);
	}
	if (*fullpath == NULL)
	{
		// ft_printf("fullpath: %s\n", fullpath);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else if (access(*fullpath, X_OK) != 0)
	{
		perror("");
		delete_free(fullpath);
	}
}

int	parser(int ac, char **av, char **envp, t_arg *ar)
{
	ar_init(ar);
	if (ac != 5)
	{
		ft_putstr_fd("Error: Bad arguments.\n", 2);
		ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
		exit(1);
	}
	while (*envp != NULL && ar->paths == NULL)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			ar->paths = ft_split((*envp + 5), ':');
		envp++;
	}
	ar->cmd1_arr = ft_split(av[2], ' ');
	ar->cmd2_arr = ft_split(av[3], ' ');
	// if (*(ar->cmd1_arr) == NULL)
	// 	ft_putstr_fd("permission denied: \n", 2);
	// if (*(ar->cmd2_arr) == NULL)
	// 	ft_putstr_fd("permission denied: \n", 2);
	find_fullpath(ar, *(ar->cmd1_arr), &ar->cmd1_path);
	find_fullpath(ar, *(ar->cmd2_arr), &ar->cmd2_path);
	if (!ar->cmd1_path || !ar->cmd2_path)
		return (0);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_arg	ar;
	int		pid1;
	int		pid2;
	int		fd[2];

	if (!parser(ac, av, envp, &ar))
		free_exit(&ar);
	if (pipe(fd) == -1)
		free_exit(&ar);
	pid1 = fork();
	if (pid1 < 0)
		free_exit(&ar);
	if (pid1 == 0)
		child_one(ar, av, envp, fd);
	pid2 = fork();
	if (pid2 < 0)
		free_exit(&ar);
	if (pid2 == 0)
		child_two(ar, av, envp, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	ar_free(&ar);
	return (0);
}
