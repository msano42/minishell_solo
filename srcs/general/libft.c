#include <stddef.h>
#include <unistd.h>
#include <errno.h>

#include "wrappers.h" // memory

#include <string.h> // ft_memmove

int	ft_isalpha(int c)
{
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ('0' <= c && c <= '9'));
}

int	ft_isprint(int c)
{
	return ('\040' <= c && c <= '\176');
}

char	*ft_strchr(const char *str, int c)
{
	if (!str)
		return (NULL);
	while (1)
	{
		if (*str == (char)c)
			return ((char *)str);
		if (*str == '\0')
			return (NULL);
		str++;
	}
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_memset(void *dst, int c, size_t len)
{
	size_t	i;

	if (!dst)
		return (NULL);
	i = 0;
	while (i < len)
	{
		((unsigned char *)dst)[i] = (unsigned char)c;
		i++;
	}
	return (dst);
}

char	*ft_strdup(const char *str)
{
	size_t	i;
	char	*ret;

	if (!str)
		return (NULL);
	ret = xmalloc(ft_strlen(str)+ 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

char	*ft_itoa(int n)
{
	int			i;
	long		n_long;
	char		buf[12];
	const int	neg = (n < 0);

	n_long = n;
	if (n_long < 0)
		n_long = -n_long;
	buf[11] = '\0';
	i = 10;
	while (n_long >= 0)
	{
		buf[i] = (n_long % 10) + '0';
		n_long = n_long / 10;
		i--;
		if (n_long == 0)
			break ;
	}
	if (neg)
		buf[i] = '-';
	return (ft_strdup(&buf[i + !neg]));
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	if (!len || dst == src)
		return (dst);
	i = 0;
	if (dst < src)
	{
		while (i < len)
		{
			((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		while (len > 0)
		{
			((unsigned char *)dst)[len - 1] = ((unsigned char *)src)[len - 1];
			len--;
		}
	}
	return (dst);
}

void	ft_itoa_no_alloc(int n, char buf[12])
{
	int			i;
	long		n_long;
	const int	neg = (n < 0);

	n_long = n;
	if (n_long < 0)
		n_long = -n_long;
	buf[11] = '\0';
	i = 10;
	while (n_long >= 0)
	{
		buf[i] = (n_long % 10) + '0';
		n_long = n_long / 10;
		i--;
		if (n_long == 0)
			break ;
	}
	if (neg)
		buf[i] = '-';
	ft_memmove(buf, &buf[i + !neg], (12 - (i + !neg)));
}

ssize_t	ft_putstr_fd(const char *str, int fd)
{
	if (!str)
		return (0);
	errno = 0;
	return (xwrite(fd, str, ft_strlen(str)));
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	char	*ret;

	if (ft_strlen(s1) == SIZE_MAX || \
		SIZE_MAX - ft_strlen(s1) - 1 < ft_strlen(s2))
		return (NULL);
	ret = xmalloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		ret[i + j] = s2[j];
		j++;
	}
	ret[i + j] = '\0';
	return (ret);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*ret;

	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	ret = xmalloc(new_size);
	if (!ret)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr)
	{
		if (new_size < old_size)
			ft_memmove(ret, ptr, new_size);
		else
			ft_memmove(ret, ptr, old_size);
	}
	free(ptr);
	return (ret);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while ((i + 1) < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
