#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

static bool	ft_isspace(int c)
{
	c = (unsigned char)c;
	return (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ');
}

static const char	*skip_whitespace(const char *s)
{
	while (ft_isspace(*s))
		s++;
	return (s);
}

static int	is_negative(const char **s)
{
	if (**s == '-')
	{
		(*s)++;
		return (1);
	}
	else if (**s == '+')
	{
		(*s)++;
	}
	return (0);
}

static int	parse_base(const char **s, int base)
{
	if ((base == 0 || base == 16) && **s == '0'
		&& ((*s)[1] == 'x' || (*s)[1] == 'X'))
	{
		(*s) += 2;
		base = 16;
	}
	if (base == 0)
	{
		if (**s == '0')
			base = 8;
		else
			base = 10;
	}
	return (base);
}

static int	get_digit_value(unsigned char c)
{
	if ((c >= '0' && c <= '9'))
		return (c - '0');
	else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
	{
		if ((c >= 'A' && c <= 'Z'))
			return (c - 'A' + 10);
		else
			return (c - 'a' + 10);
	}
	return (-1);
}

static uintmax_t	parse_integer_core(const char **s, int base, int *any,
	uintmax_t cutoff) {
	uintmax_t			integer_part;
	unsigned char		c;
	int					digit_value;
	uintmax_t			cutlim;

	integer_part = 0;
	cutlim = cutoff % base;
	while (1)
	{
		c = **s;
		digit_value = get_digit_value(c);
		if (digit_value == -1 || digit_value >= base)
			break ;
		if (*any < 0 || (integer_part > cutoff
				|| (integer_part == cutoff && (uintmax_t)digit_value > cutlim)))
			*any = -1;
		else
		{
			*any = 1;
			integer_part = integer_part * base + digit_value;
		}
		(*s)++;
	}
	return (integer_part);
}

static uintmax_t	parse_integer(const char **s, int base, int neg, int *any)
{
	uintmax_t	cutoff;

	if (neg)
		cutoff = (uintmax_t) - (INTMAX_MIN + INTMAX_MAX) + INTMAX_MAX;
	else
		cutoff = INTMAX_MAX;
	cutoff /= base;
	return (parse_integer_core(s, base, any, cutoff));
}

static intmax_t	assign_intmax(int neg)
{
	if (neg)
		return (INTMAX_MIN);
	return (INTMAX_MAX);
}

static void	update_endptr(char **endptr, const char *s,
	const char *nptr, int any)
{
	if (endptr == NULL)
		return ;
	if (any)
		*endptr = (char *)s;
	else
		*endptr = (char *)nptr;
}

intmax_t	ft_strtoimax(const char *nptr, char **endptr, int base)
{
	const char		*s;
	int				neg;
	uintmax_t		integer_part;
	int				any;

	s = skip_whitespace(nptr);
	neg = is_negative(&s);
	base = parse_base(&s, base);
	any = 0;
	if (base < 2 || base > 36)
		errno = EINVAL;
	integer_part = parse_integer(&s, base, neg, &any);
	if (any < 0)
	{
		integer_part = assign_intmax(neg);
		errno = ERANGE;
	}
	else if (!any)
		errno = EINVAL;
	else if (neg)
		integer_part = -integer_part;
	update_endptr(endptr, s, nptr, any);
	return (integer_part);
}

// #include <inttypes.h>
// int main()
// {
// 	char *s = "    321g2 ";
// 	char *end = "";
// 	printf("%jd\n", ft_strtoimax(s, &end, 4));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 4));
// 	printf("end: %s\n", end);
// 	printf("\n============\n\n");
// 	printf("%jd\n", ft_strtoimax(s, &end, 10));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 10));
// 	printf("end: %s\n", end);
// 	printf("\n============\n\n");
// 	s = "0x236f9   ";
// 	printf("%jd\n", ft_strtoimax(s, &end, 15));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 15));
// 	printf("end: %s\n", end);
// 	printf("\n============\n\n");
// 	printf("%jd\n", ft_strtoimax(s, &end, 0));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 0));
// 	printf("end: %s\n", end);
// 	printf("\n============\n\n");
// 	s = "   -0x2A36E9   ";
// 	printf("%jd\n", ft_strtoimax(s, &end, 16));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 16));
// 	printf("end: %s\n", end);
// 	printf("\n============\n\n");
// 	s = "		04434   ";
// 	printf("%jd\n", ft_strtoimax(s, &end, 0));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 0));
// 	printf("end: %s\n", end);
// 	printf("\n============\n\n");
// 	s = "		9223372036854775809   ";
// 	printf("%jd\n", ft_strtoimax(s, &end, 0));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 0));
// 	printf("end: %s\n", end);
// 	printf("\n============\n\n");
// 	s = "		-9223372036854775809   ";
// 	printf("%jd\n", ft_strtoimax(s, &end, 15));
// 	printf("end: %s\n", end);
// 	printf("%jd\n", strtoimax(s, &end, 15));
// 	printf("end: %s\n", end);
// }