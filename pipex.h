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
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

typedef struct  s_arg
{
    char        **paths;
    char        *file1;
    char        *file2;
    char        *cmd1;
    char        *cmd2;
    char        *cmd1_path;
    char        *cmd2_path;
    char        **cmd1_arr;
    char        **cmd2_arr;
    int         fd_src;
    int         fd_dst;
}               t_arg;

char	**ft_split(char const *s, char c);
char	**copywords(int wordcount, char **dest, char const *s, char c);
int	countwords(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);

#endif