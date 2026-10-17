/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:19:07 by psalame           #+#    #+#             */
/*   Updated: 2025/11/08 14:48:34 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"
#include <stdlib.h>
#include <unistd.h>
#include "../ssl_algorithm_Int.h"

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


static uint32_t F(uint32_t B, uint32_t C, uint32_t D, int i)
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

static uint32_t	red_box(uint32_t x, int i, uint32_t B)
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
	int			j;

	for (int i = 0; i < 64; i++) {
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

static void md5_hash_constant_filler(uint32_t *hash) {
	hash[0] = 0x67452301;
	hash[1] = 0xEFCDAB89;
	hash[2] = 0x98BADCFE;
	hash[3] = 0x10325476;
}

uint32_t	*ft_ssl_md5(uint8_t *msg, uint64_t len)
{
	return ssl_32(msg, len, &digest_loop, 4, &md5_hash_constant_filler, 0);
}

uint32_t	*ft_ssl_md5_fd(int fd, int print_fd)
{
	return ssl_32_fd(fd, print_fd, &digest_loop, 4, &md5_hash_constant_filler, 0);
}
