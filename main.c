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

void	delete_free(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

void	child_one(t_arg ar, int fd[2])
{
	int	exec_status;

	dup2(ar.fd_src, STDIN_FILENO);
	close(ar.fd_src);
	dup2(STDOUT_FILENO, fd[0]);
	close(fd[1]);
	exec_status = execve(ar.cmd1_path, ar.cmd1_arr, NULL);
	if (exec_status == -1)
		perror("cmd1 process failed");
}

void	child_two(t_arg ar, int fd[2])
{
	int	exec_status;

	dup2(fd[1], STDIN_FILENO);
	close(fd[0]);
	dup2(ar.fd_dst, STDOUT_FILENO);
	close(ar.fd_dst);
	exec_status = execve(ar.cmd2_path, ar.cmd2_arr, NULL);
	if (exec_status == -1)
		perror("cmd2 process failed");
}

int	cmd_fullpath(t_arg *ar)
{
	char	*cmd1_path_tmp;
	char	*cmd2_path_tmp;
	int		i;

	i = 0;
	while (!(ar->cmd1_path && ar->cmd2_path) && ar->paths[i++])
	{
		cmd1_path_tmp = ft_strjoin(ar->paths[i], "/");
		cmd2_path_tmp = ft_strjoin(ar->paths[i], "/");
		if (ar->cmd1_path == NULL)
			ar->cmd1_path = ft_strjoin(cmd1_path_tmp, ar->cmd1_arr[0]);
		if (access(ar->cmd1_path, X_OK) != 0)
			delete_free(&ar->cmd1_path);
		free(cmd1_path_tmp);
		if (ar->cmd2_path == NULL)
			ar->cmd2_path = ft_strjoin(cmd2_path_tmp, ar->cmd2_arr[0]);
		if (access(ar->cmd2_path, X_OK) != 0)
			delete_free(&ar->cmd2_path);
		free(cmd2_path_tmp);
	}
	if (ar->cmd1_path == NULL)
		printf("cmd1 path not found\n");
	if (ar->cmd2_path == NULL)
		printf("cmd2 path not found\n");
	return (1);
}

void	ar_init(t_arg *ar)
{
	ar->paths = NULL; // malloc, free
	ar->file1 = NULL; // malloc, free
	ar->file2 = NULL; // malloc, free
	ar->cmd1 = NULL; // malloc, free
	ar->cmd2 = NULL; // malloc, free
	ar->cmd1_path = NULL; // malloc
	ar->cmd2_path = NULL; // malloc
	ar->cmd1_arr = NULL; // malloc, free
	ar->cmd2_arr = NULL; // malloc, free
}

int	parser(int ac, char **av, char **envp, t_arg *ar)
{
	ar_init(ar);
	if (ac != 5)
		return (printf("Please enter 4 arguments.\n") && 0);
	while (*envp != NULL && ar->paths == NULL)
	{
		if (strncmp(*envp, "PATH=", 5) == 0)
			ar->paths = ft_split((*envp + 5), ':');
		envp++;
	}
	ar->file1 = strdup(av[1]); // malloc
	ar->file2 = strdup(av[4]); // malloc
	ar->cmd1 = strdup(av[2]); // malloc
	ar->cmd2 = strdup(av[3]); // malloc
	ar->cmd1_arr = ft_split(ar->cmd1, ' '); // malloc
	ar->cmd2_arr = ft_split(ar->cmd2, ' '); // malloc
	ar->fd_src = open(ar->file1, O_RDONLY);
	ar->fd_dst = open(ar->file2, O_WRONLY);
	// printf("%s\n", ar->file1);
	// printf("%s\n", ar->file2);
	if (ar->fd_src == -1)
		return (printf("File1 is not accessible. Exit."));
	if (ar->fd_dst == -1)
		return (printf("File2 is not accessible. Exit."));
	if (!cmd_fullpath(ar))
		return (0);
	return (1);
}

void	ar_free(t_arg *ar)
{
	int	i;

	i = 0;
	while (ar->paths[i++])
		free(ar->paths[i]);
	free(ar->paths);
	free(ar->file1);
	free(ar->file2);
	free(ar->cmd1);
	free(ar->cmd2);
	free(ar->cmd1_path);
	free(ar->cmd2_path);
	i = 0;
	while (ar->cmd1_arr[i++])
		free(ar->cmd1_arr[i]);
	free(ar->cmd1_arr);
	i = 0;
	while (ar->cmd2_arr[i++])
		free(ar->cmd2_arr[i]);
	free(ar->cmd2_arr);
}

int	main(int ac, char **av, char **envp)
{
	t_arg	ar;
	int		pid1;
	int		pid2;
	int		fd[2];

	if (!parser(ac, av, envp, &ar))
		return (1);
	if (pipe(fd) == -1)
		return (2);
	pid1 = fork();
	if (pid1 < 0)
		return (3);
	if (pid1 == 0)
		child_one(ar, fd);
	pid2 = fork();
	if (pid2 < 0)
		return (4);
	if (pid2 == 0)
		child_two(ar, fd);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	close(fd[0]);
	close(fd[1]);
	ar_free(&ar); // malloc in parsing
	return (0);
}
/*
int	main()
{
	// int	fd = open("hello.txt", O_RDONLY);
	char *const argv[] = {"ls", "-l", "hello.txt", NULL};
	char *const	envp[] = {"/home/yrigny/bin", "/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin", "/usr/games", "/usr/local/games", "/snap/bin", NULL};
	int	n = fork();
	if (n == 0)
	{
		int fd_tmp = dup(1);
		printf("%d\n", fd_tmp);
		int	fd_dst = open("test", O_RDWR);
		dup2(fd_dst, 1);
		close(fd_dst);
		int	exec = execve("ls", argv, NULL);
		if (exec < 0)
			perror("child process failed");
		dup2(fd_tmp, 1);
		close(fd_tmp);
		printf("child process executed\n");
	}
	wait(NULL);
	printf("child process terminated\n");
	close(fd);
	return (0);
}*/
