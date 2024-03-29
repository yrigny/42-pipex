/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 16:37:47 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/11 18:41:55 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_exit(t_list **cmds, int status)
{
	t_list	*tmp;
	t_proc	*child;
	int		i;

	if (!cmds)
		return ;
	while (*cmds)
	{
		tmp = (*cmds)->next;
		child = (*cmds)->content;
		i = -1;
		while (child->cmd_arr[++i])
			free(child->cmd_arr[i]);
		free(child->cmd_arr);
		free(child->path);
		free(child);
		free(*cmds);
		*cmds = tmp;
	}
	exit(status);
}

void	find_path(char **paths, t_proc *child)
{
	char	*cmd_path_tmp;
	int		i;

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
}

void	parse_cmds(int ac, char **av, char **paths, t_list **cmds)
{
	int		nb_cmds;
	t_list	*new_cmd;
	t_proc	*child;

	nb_cmds = ac - 3;
	new_cmd = NULL;
	child = NULL;
	while (nb_cmds--)
	{
		child = malloc(sizeof(t_proc));
		if (!child)
			free_exit(cmds, EXIT_FAILURE);
		child->cmd_arr = ft_split(av[ac - nb_cmds - 2], ' ');
		if (child->cmd_arr[0] && access(child->cmd_arr[0], F_OK) == 0)
			child->path = ft_strdup(child->cmd_arr[0]);
		else
			find_path(paths, child);
		new_cmd = ft_lstnew((void *)child);
		if (!new_cmd)
			free_exit(cmds, EXIT_FAILURE);
		ft_lstadd_back(cmds, new_cmd);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_list	*cmds;
	char	**paths;
	int		i;

	if (ac < 5)
	{
		ft_putstr_fd("Error: Bad arguments.\n", 2);
		ft_putstr_fd("Ex: ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n", 1);
		exit(EXIT_FAILURE);
	}
	cmds = NULL;
	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	paths = ft_split((*envp + 5), ':');
	parse_cmds(ac, av, paths, &cmds);
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	pipex(cmds, ac, av);
	free_exit(&cmds, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
