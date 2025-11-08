/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl_algorithm_Int.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 13:59:00 by psalame           #+#    #+#             */
/*   Updated: 2025/11/08 14:48:04 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_ALGORITHM_INT_H
# define SSL_ALGORITHM_INT_H
# include <stdint.h>

static inline uint32_t	rotate_left(uint32_t x, unsigned char c)
{
	return (x << c) | (x >> (32 - c));
}

static inline uint32_t	rotate_right(uint32_t x, unsigned char c)
{
	return (x >> c) | (x << (32 - c));
}
uint64_t	pad_msg(uint8_t *msg, uint64_t len, uint8_t **new_msg, uint64_t fullLen, char endian);

uint32_t *ssl_32(uint8_t *msg,
				uint64_t len,
				void (*digest_fct)(uint32_t *, uint32_t *),
				char hash_size,
				void (*hash_kfiller)(uint32_t *),
				char endian);

uint32_t	*ssl_32_fd(int fd,
						int print_fd,
						void (*digest_fct)(uint32_t *, uint32_t *),
						char hash_size,
						void (*hash_kfiller)(uint32_t *),
						char endian);

#endif
