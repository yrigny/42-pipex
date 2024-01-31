#include "pipex.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	cpylen;
	size_t	i;

	len = 0;
	while (src[len])
		len++;
	if (size < 1)
		return (len);
	if (size - 1 >= len)
		cpylen = len;
	if (size - 1 < len)
		cpylen = size - 1;
	i = 0;
	while (i < cpylen)
	{
		*dst++ = *src++;
		i++;
	}
	*dst = '\0';
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		len1;
	int		len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = malloc(len1 + len2 + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len1 + 1);
	ft_strlcpy(str + len1, s2, len2 + 1);
	return (str);
}