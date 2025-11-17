 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 13:59:42 by psalame           #+#    #+#             */
/*   Updated: 2025/11/08 15:59:24 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft/header/libft.h"
#include <stdint.h>

uint64_t	pad_msg(uint8_t *msg, uint64_t len, uint8_t **new_msg, uint64_t fullLen, char endian)
{
	uint64_t targetLen = len + 1;
	while ((targetLen % 64) != 56)
		targetLen++;
	targetLen += 8;

	*new_msg = malloc(targetLen);
	if (!*new_msg)
		return (0);
		
	ft_memcpy(*new_msg, msg, len);
	(*new_msg)[len] = 0x80; // 0b10000000
	ft_memset(*new_msg + len + 1, 0, targetLen - len - 1 - 8);

	for (uint64_t i = 0; i < 8; i++) {
		if (endian)
			(*new_msg)[targetLen - 8 + i] = (uint8_t)((fullLen * 8) >> (8 * (7 - i))) & 0xFF;
		else
			(*new_msg)[targetLen - 8 + i] = (uint8_t)((fullLen * 8) >> (8 * i)) & 0xFF;
	}

	return targetLen;
}

static inline void convert_block_big_endian(uint32_t *block) {
	uint32_t tmp[16];
	for (int i = 0; i < 16; i++) {
		tmp[i] = ((block[i] >> 24) & 0x000000FF) |
				  ((block[i] >> 8) & 0x0000FF00) |
				  ((block[i] << 8) & 0x00FF0000) |
				  ((block[i] << 24) & 0xFF000000);
	}
	ft_memcpy(block, tmp, 64);
}

uint32_t *ssl_32(uint8_t *msg,
				uint64_t len,
				void (*digest_fct)(uint32_t *, uint32_t *),
				char hash_size,
				void (*hash_kfiller)(uint32_t *),
				char endian) {
	uint8_t	*new_msg;
	uint64_t new_len = pad_msg(msg, len, &new_msg, len, endian);
	if (!new_msg)
		return (NULL);

	uint32_t	*hash = malloc(hash_size * sizeof(uint32_t));
	if (!hash) {
		free(new_msg);
		return (NULL);
	}

	(*hash_kfiller)(hash);

	for (uint64_t i = 0; i < new_len; i += 64) {
		if (endian)
			convert_block_big_endian((uint32_t *)(new_msg + i));
		(*digest_fct)((uint32_t *)(new_msg + i), hash);
	}

	free(new_msg);

	if (endian) {
		for (char i = 0; i < hash_size; i++) {
			hash[i] = ((hash[i] >> 24) & 0x000000FF) |
					  ((hash[i] >> 8) & 0x0000FF00) |
					  ((hash[i] << 8) & 0x00FF0000) |
					  ((hash[i] << 24) & 0xFF000000);
		}
	}
		

	return (hash);
}

static void print_no_nl(char *str, ssize_t len, int fd) {
	ssize_t i = 0;
	while (i < len) {
		while (i < len && str[i] != '\n') {
			i++;
		}
		write(fd, str, i);
		str += i + 1;
		len -= i + 1;
		i = 0;
	}
}

uint32_t	*ssl_32_fd(int fd,
						int print_fd,
						void (*digest_fct)(uint32_t *, uint32_t *),
						char hash_size,
						void (*hash_kfiller)(uint32_t *),
						char endian)
{
	uint32_t	*hash = malloc(hash_size * sizeof(uint32_t));
	if (!hash)
		return (NULL);

	(*hash_kfiller)(hash);

	uint64_t	totalReadBytes = 0;
	char		bufferIndex = 0;
	char		readBytes = 0;
	uint8_t		buffer[64];

	do {
		readBytes = read(fd, buffer + bufferIndex, 64 - bufferIndex);
		if (readBytes < 0) {
			free(hash);
			return (NULL);
		}
		if (print_fd > 0 && readBytes > 0)
			print_no_nl(buffer + bufferIndex, readBytes, print_fd);
		bufferIndex += readBytes;

		if (bufferIndex == 64) {
			if (endian)
				convert_block_big_endian((uint32_t *)buffer);
			(*digest_fct)((uint32_t *)buffer, hash);
			totalReadBytes += bufferIndex;
			bufferIndex = 0;
		} else if (readBytes == 0) {
			uint8_t	*new_msg;
			uint64_t new_len = pad_msg(buffer, bufferIndex, &new_msg, totalReadBytes + bufferIndex, endian);
			if (!new_msg) {
				free(hash);
				return (NULL);
			}
			for (uint64_t i = 0; i < new_len; i += 64) {
				if (endian)
					convert_block_big_endian((uint32_t *)(new_msg + i));
				(*digest_fct)((uint32_t *)(new_msg + i), hash);
			}
			free(new_msg);
		}
	} while (readBytes > 0);

	if (endian) {
		for (char i = 0; i < hash_size; i++) {
			hash[i] = ((hash[i] >> 24) & 0x000000FF) |
					  ((hash[i] >> 8) & 0x0000FF00) |
					  ((hash[i] << 8) & 0x00FF0000) |
					  ((hash[i] << 24) & 0xFF000000);
		}
	}

	return (hash);
}