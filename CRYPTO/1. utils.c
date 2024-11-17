/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:36:40 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 14:53:29 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

void	ft_putchar(char c)
{
	printf("%c", c);
}

int	ft_strlen(char *str)
{
	int	i = 0;

	if (str == NULL)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	ft_putnbr(int n)
{
	char	r;

	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		ft_putchar('\n');
		return ;
	}
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	if (n > 9)
		ft_putnbr(n / 10);
	r = (n % 10 ) + '0';
	ft_putchar(r);
}

void	print_line(char c, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		ft_putchar(c);
		i++;
	}
	ft_putchar('\n');
}

static int ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	is_snake_case(const char *str)
{
	int	i = 0;

	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int is_valid_date(const char *date)
{
	int i = 0;

	if (ft_strlen((char *)date) != 10)
		return (0);

	while (date[i])
	{
		if (i == 4 || i == 7)
		{
			if (date[i] != '-')
				return (0);
		}
		else if (date[i] < '0' || date[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strdup(char *str)
{
	char	*duplicate;
	int		len;
	int		i;

	if (str == NULL)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	duplicate = (char *)malloc(sizeof(char) * (len + 1));
	if (duplicate == NULL)
		return (NULL);
	while (str[i])
	{
		duplicate[i] = str[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}
