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
		int	exec = execve("/usr/bin/ls", argv, envp);
		if (exec < 0)
			perror("child process failed");
		dup2(fd_tmp, 1);
		close(fd_tmp);
		printf("child process executed\n");
	}
	else
	{
		wait(NULL);
		printf("child process terminated\n");
	}
	// close(fd);
	return (0);
}
