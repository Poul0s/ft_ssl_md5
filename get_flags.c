/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:58:52 by psalame           #+#    #+#             */
/*   Updated: 2025/11/08 12:37:55 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include <unistd.h>
#include "libft/header/ft_string.h"

int	get_flags(int ac, char **av, int *optind)
{
	const char flagsList[][2] = {
		{'p', FLAG_ECHO},
		{'q', FLAG_QUIET},
		{'r', FLAG_REVERSE},
		{'s', FLAG_SUM},
		{0, 0}
	};
	
	int	flags = 0;
	while (*optind < ac) {
		char *current = av[*optind];
		if (current[0] != '-'
			|| (current[0] == '-' && current[1] == '\0')
			|| (current[0] == '-' && current[1] == '-')
			|| flags & FLAG_SUM)
			break ;
		(*optind)++;
		for (int i = 1; current[i] != '\0'; i++) {
			char found = 0;
			for (int j = 0; flagsList[j][0] != 0; j++) {
				if (current[i] == flagsList[j][0]) {
					flags |= flagsList[j][1];
					found = 1;
					break ;
				}
			}
			if (!found) {
				write(2, av[0], ft_strlen(av[0]));
				write(2, ": ", 2);
				write(2, av[1], ft_strlen(av[1]));
				write(2, ": invalid option -- '", 21);
				write(2, &current[i], 1);
				write(2, "'\n", 2);
				return UNKNOWN_FLAG;
			}
		}
	}
	return (flags);
}
