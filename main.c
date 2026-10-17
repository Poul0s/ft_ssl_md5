/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 21:10:22 by psalame           #+#    #+#             */
/*   Updated: 2025/11/17 11:14:59 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "libft/header/ft_string.h"
#include "libft/header/ft_print.h"

static void print_hex(const uint8_t *d, int size) {
	static const char hexd[] = "0123456789abcdef";
	for (int i = 0; i < size * 4; i++) {
		write(1, hexd + (d[i] >> 4), 1);
		write(1, hexd + (d[i] & 0x0F), 1);
	}
}

static void print_upper(const char *str) {
	while (*str)
		ft_putchar_fd(ft_toupper(*str++), 1);
}

static void print_res(const uint8_t *d, const char *input, const char *algo_name, int algo_size, int flags, int is_file, int is_stdin) {
	if (!(flags & (FLAG_QUIET | FLAG_REVERSE))) {
		if (!is_stdin) {
			print_upper(algo_name);
			ft_putchar_fd(' ', 1);
		}
		ft_printf(is_file ? "(%s)" : "(\"%s\")", input);
		if (!is_stdin)
			ft_putstr_fd(" = ", 1);
		else
			ft_putstr_fd("= ", 1);
	}
	print_hex(d, algo_size);
	if (flags & FLAG_REVERSE && !(flags & FLAG_QUIET))
		ft_printf(is_file ? " %s\n" : " \"%s\"\n", input);
	else
		ft_putchar_fd('\n', 1);
}

static void clean_exec_name(char *exec_name) {
	char *last_slash = ft_strrchr(exec_name, '/');
	if (last_slash)
		ft_memmove(exec_name, last_slash + 1, ft_strlen(last_slash + 1) + 1);
}

int	main(int ac, char **av) {
	int		flags;
	void	*(*algorithm)(uint8_t *msg, uint64_t len);
	void	*(*algorithm_fd)(int fd, int print_fd);
	int		hash_size;
	struct s_ssl_algos algos[] = {
		{"md5", (void *(*)(uint8_t *, uint64_t))&ft_ssl_md5, (void *(*)(int, int))&ft_ssl_md5_fd, 4},
		{"sha256", (void *(*)(uint8_t *, uint64_t))&ft_ssl_sha256, (void *(*)(int, int))&ft_ssl_sha256_fd, 8},
		{0, 0, 0, 0}
	};
	int		optind = 2;

	clean_exec_name(av[0]);
	if (ac == 1) {
		ft_printf("usage: %s command [flags] [file/string]\n", av[0]);
		return 0;
	}

	algorithm = NULL;
	for (int i = 0; algos[i].name != 0; i++) {
		if (ft_strncmp(av[1], algos[i].name, ft_strlen(algos[i].name) + 1) == 0) {
			algorithm = algos[i].algo;
			algorithm_fd = algos[i].algo_fd;
			hash_size = algos[i].hash_size;
			break;
		}
	}

	if (algorithm == NULL) {
		ft_dprintf(2, "%s: Error: '%s' is an invalid command.\n\nCommands:\nmd5\nsha256\n\nFlags:\n-p -q -r -s\n", av[0], av[1]);
		return 1;
	}

	
	flags = get_flags(ac, av, &optind);

	if (flags == UNKNOWN_FLAG)
		return 1;

	if (flags & FLAG_SUM && optind >= ac) {
		ft_dprintf(2, "%s: %s: option requires an argument -- 's'\n", av[0], av[1]);
		return 1;
	}

	// priority call order:
	// -p flag
	// -s flag
	// if no -p, -s and no files, read from stdin
	// files
	
	if (flags & FLAG_ECHO) {
		if (!(flags & FLAG_QUIET))
			ft_putstr_fd("(\"", 1);
		uint32_t *hash = (*algorithm_fd)(0, 1 + ((flags & FLAG_QUIET) == 0));
		if (!(flags & FLAG_QUIET))
			ft_putstr_fd("\")= ", 1);
		else
			ft_putchar_fd('\n', 1);
		if (!hash) {
			if (!(flags & FLAG_QUIET))
				ft_putchar_fd('\n', 1);
			ft_dprintf(2, "%s: %s: failed to read from stdin\n", av[0], av[1]);
		} else {
			print_hex((const uint8_t *)hash, hash_size);
			ft_putchar_fd('\n', 1);
			free(hash);
		}
	}

	if (flags & FLAG_SUM) {
		size_t len = ft_strlen(av[optind]);
		uint32_t *hash = (*algorithm)((uint8_t *)av[optind], len);
		if (!hash) {
			ft_dprintf(2, "%s: %s: failed to process string\n", av[0], av[1]);
		} else {
			print_res((const uint8_t *)hash, av[optind], av[1], hash_size, flags, 0, 0);
			free(hash);
		}
	}
	
	if (!(flags & (FLAG_ECHO | FLAG_SUM)) && optind >= ac) {
		uint32_t *hash = (*algorithm_fd)(0, -1);
		if (!hash) {
			ft_dprintf(2, "%s: %s: failed to read from stdin\n", av[0], av[1]);
		} else {
			print_res((const uint8_t *)hash, "stdin", av[1], hash_size, flags, 1, 1);
			free(hash);
		}
	}

	optind += (flags & FLAG_SUM) ? 1 : 0;
	while (av[optind]) {
		char is_stdin = av[optind][0] == '-' && (av[optind][1] == '\0' || (av[optind][1] == '-' && av[optind][2] == '\0'));
		int fd = is_stdin ? 0 : open(av[optind], O_RDONLY);
		if (fd < 0) {
			ft_dprintf(2, "%s: %s: %s: %s\n", av[0], av[1], av[optind], strerror(errno));
		} else {
			uint32_t *hash = (*algorithm_fd)(fd, -1);
			if (!hash) {
				ft_dprintf(2, "%s: %s: %s: failed to read file\n", av[0], av[1], av[optind]);
			} else {
				print_res((const uint8_t *)hash, av[optind], av[1], hash_size, flags, 1, 0);
				free(hash);
			}
			if (fd > 2)
				close(fd);
		}
		optind++;
	}
	
	return 0;
}