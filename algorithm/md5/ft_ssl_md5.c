/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:19:07 by psalame           #+#    #+#             */
/*   Updated: 2025/11/07 21:44:16 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"
#include <stdlib.h>
#include <unistd.h>

static char s[] = {
	 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

static uint32_t K[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static inline uint32_t	rotate_left(uint32_t x, unsigned char c)
{
	return (x << c) | (x >> (32 - c));
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (dest == NULL && src == NULL && n != 0)
		return (dest);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		((unsigned char *)s)[i++] = c;
	return (s);
}

static uint64_t	pad_msg(uint8_t *msg, uint64_t len, uint8_t **new_msg, uint64_t fullLen)
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

	for (uint64_t i = 0; i < 8; i++)
		(*new_msg)[targetLen - 8 + i] = (uint8_t)((fullLen * 8) >> (8 * i)) & 0xFF;

	return targetLen;
}

static uint32_t F(uint32_t B, uint32_t C, uint32_t D, char i)
{
	if (i < 16)
		return (B & C) | ((~B) & D);
	else if (i < 32)
		return (D & B) | ((~D) & C);
	else if (i < 48)
		return B ^ C ^ D;
	else
		return C ^ (B | (~D));
}

static uint32_t	red_box(uint32_t x, char i, uint32_t B)
{
	return rotate_left((x + K[i]), s[i]) + B;
}

static void	digest_loop(uint32_t *msg, uint32_t *hash)
{
	uint32_t	A = hash[0];
	uint32_t	B = hash[1];
	uint32_t	C = hash[2];
	uint32_t	D = hash[3];
	uint32_t	combin;
	char		j;

	for (char i = 0; i < 64; i++) {
		if (i < 16)
			j = i;
		else if (i < 32)
			j = (5 * i + 1) % 16;
		else if (i < 48)
			j = (3 * i + 5) % 16;
		else
			j = (7 * i) % 16;

		combin = red_box(F(B, C, D, i) + A + msg[j], i, B);
		A = D;
		D = C;
		C = B;
		B = combin;
	}
	hash[0] += A;
	hash[1] += B;
	hash[2] += C;
	hash[3] += D;
}

uint32_t	*ft_ssl_md5(uint8_t *msg, uint64_t len)
{
	uint8_t	*new_msg;
	uint64_t new_len = pad_msg(msg, len, &new_msg, len);
	if (!new_msg)
		return (NULL);

	uint32_t	*hash = malloc(4 * sizeof(uint32_t));
	if (!hash) {
		free(new_msg);
		return (NULL);
	}

	hash[0] = 0x67452301;
	hash[1] = 0xEFCDAB89;
	hash[2] = 0x98BADCFE;
	hash[3] = 0x10325476;

	for (uint64_t i = 0; i < new_len; i += 64)
		digest_loop((uint32_t *)(new_msg + i), hash);

	free(new_msg);
	return (hash);
}

#include <stdio.h>
uint32_t	*ft_ssl_md5_fd(int fd)
{
	uint32_t	*hash = malloc(4 * sizeof(uint32_t));
	if (!hash)
		return (NULL);

	hash[0] = 0x67452301;
	hash[1] = 0xEFCDAB89;
	hash[2] = 0x98BADCFE;
	hash[3] = 0x10325476;

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
		bufferIndex += readBytes;

		if (bufferIndex == 64) {
			digest_loop((uint32_t *)buffer, hash);
			totalReadBytes += bufferIndex;
			bufferIndex = 0;
		} else if (readBytes == 0) {
			uint8_t	*new_msg;
			uint64_t new_len = pad_msg(buffer, bufferIndex, &new_msg, totalReadBytes + bufferIndex);
			if (!new_msg) {
				free(hash);
				return (NULL);
			}
			for (uint64_t i = 0; i < new_len; i += 64)
				digest_loop((uint32_t *)(new_msg + i), hash);
			free(new_msg);
		}
	} while (readBytes > 0);

	return (hash);
}
