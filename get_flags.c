/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:58:52 by psalame           #+#    #+#             */
/*   Updated: 2025/11/07 23:18:27 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include <string.h> // todo rm

int	get_flags(int ac, char **av, int *optind)
{
	/// parse flags from (av + *optind) until:
	/// 1) non-flag argument is found V
	/// 2) all arguments are parsed V
	/// 3) an unknown flag is found V
	/// 4) arg is '-' or '--' so read stdin V
	/// 5) found -s flag so next argument is the sum argument V
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
				write(2, av[0], strlen(av[0])); // todo use ft_strlen
				write(2, ": ", 2);
				write(2, av[1], strlen(av[1])); // todo use ft_strlen
				write(2, ": invalid option -- '", 21);
				write(2, &current[i], 1);
				write(2, "'\n", 2);
				return UNKNOWN_FLAG;
			}
		}
	}
	return (flags);
}
