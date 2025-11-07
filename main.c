/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 21:10:22 by psalame           #+#    #+#             */
/*   Updated: 2025/11/07 23:23:53 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

static size_t ft_strlen(const char *s) {
	const char *start = s;
	while (*s++)
		;
	return s - start - 1;
}

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*us1;
	unsigned char	*us2;

	us1 = (unsigned char *)s1;
	us2 = (unsigned char *)s2;
	i = 0;
	while (i < n && us1[i] && us2[i])
	{
		if (us1[i] != us2[i])
			return (us1[i] - us2[i]);
		i++;
	}
	if (i < n)
		return (us1[i] - us2[i]);
	return (0);
}

static void print_hex16(const uint8_t *d) {
	static const char hexd[] = "0123456789abcdef";
	for (int i = 0; i < 16; i++) {
		write(1, hexd + (d[i] >> 4), 1);
		write(1, hexd + (d[i] & 0x0F), 1);
	}
	write(1, "\n", 1);
}

int	main(int ac, char **av) {
	int		flags;
	char	algorithm_type;
	int		optind = 2;

	if (ac == 1) {
		write(1, "usage: ", 7);
		write(1, av[0], ft_strlen(av[0]));
		write(1, " command [flags] [file/string]\n", 31);
		return 0;
	}

	if (ft_strncmp(av[1], "md5", 4) == 0) {
		algorithm_type = 0;
	} else if (ft_strncmp(av[1], "sha256", 7) == 0) {
		algorithm_type = 1;
	} else {
		write(2, av[0], ft_strlen(av[0]));
		write(2, ": Error: '", 10);
		write(2, av[1], ft_strlen(av[1]));
		write(2, "' is an invalid command.\n\nCommands:\nmd5\nsha256\n\nFlags:\n-p -q -r -n\n", 67);
		return 1;
	}

	
	flags = get_flags(ac, av, &optind);

	if (flags == UNKNOWN_FLAG)
		return 1;

	if (flags & FLAG_SUM && optind >= ac) {
		write(2, av[0], ft_strlen(av[0]));
		write(2, ": ", 2);
		write(2, av[1], ft_strlen(av[1]));
		write(2, ": option requires an argument -- 's'\n", 36);
		return 1;
	}

	// priority call order:
	// -p flag
	// -s flag

	// int fd = 0;
	// if (optind + 1 < ac) {
	// 	fd = open(av[optind + 1], O_RDONLY);
	// 	if (fd < 0) {
	// 		write(2, av[0], ft_strlen(av[0]));
	// 		write(2, ": ", 2);
	// 		write(2, av[optind], ft_strlen(av[optind]));
	// 		write(2, ": ", 2);
	// 		write(2, av[optind + 1], ft_strlen(av[optind + 1]));
	// 		write(2, ": ", 2);
	// 		char *err = strerror(errno);
	// 		write(2, err, ft_strlen(err));
	// 		write(2, "\n", 1);
	// 		return 1;
	// 	}
	// }
	
	return 0;
	
	// uint32_t *hash;
	// if (ac != 2)
	// 	hash = ft_ssl_md5_fd(0);
	// else {
	// 	size_t len = strlen(av[1]);
	// 	hash = ft_ssl_md5((uint8_t *)av[1], len);
	// }
	// if (!hash) {
	// 	printf("error\n");
	// 	return (1);
	// }
	// print_hex16((const uint8_t *)hash);
	// free(hash);
	// return (0);
}