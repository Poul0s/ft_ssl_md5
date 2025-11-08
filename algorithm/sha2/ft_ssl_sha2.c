/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_sha2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 13:58:21 by psalame           #+#    #+#             */
/*   Updated: 2025/11/08 15:00:40 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_sha2.h"
#include "../ssl_algorithm_Int.h"
#include "libft/header/libft.h"

static uint32_t K[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static inline uint32_t Ch(uint32_t E, uint32_t F, uint32_t G)
{
	return ((E & F) ^ ((~E) & G));
}

static inline uint32_t Ma(uint32_t A, uint32_t B, uint32_t C)
{
	return ((A & B) ^ (A & C) ^ (B & C));
}

static inline uint32_t Sigma0(uint32_t A)
{
	return (rotate_right(A, 2) ^ rotate_right(A, 13) ^ rotate_right(A, 22));
}

static inline uint32_t Sigma1(uint32_t E)
{
	return (rotate_right(E, 6) ^ rotate_right(E, 11) ^ rotate_right(E, 25));
}

static void	digest_loop(uint32_t *msg, uint32_t *hash)
{
	uint32_t	W[64];
	uint32_t 	hash_process_arr[16];
	uint32_t 	*hash_process = hash_process_arr;
	uint32_t 	*hash_process_prime = hash_process_arr + 8;

	ft_memcpy(hash_process, hash, 8 * sizeof(uint32_t));
	
	ft_memcpy(W, msg, 16 * sizeof(uint32_t));
	for (int i = 16; i < 64; i++) {
		uint32_t s0 = rotate_right(W[i - 15], 7) ^ rotate_right(W[i - 15], 18) ^ (W[i - 15] >> 3);
		uint32_t s1 = rotate_right(W[i - 2], 17) ^ rotate_right(W[i - 2], 19) ^ (W[i - 2] >> 10);
		W[i] = W[i - 16] + s0 + W[i - 7] + s1;
	}

	for (int i = 0; i < 64; i++) {
		hash_process_prime[1] = hash_process[0];
		hash_process_prime[2] = hash_process[1];
		hash_process_prime[3] = hash_process[2];

		hash_process_prime[0] = Ch(hash_process[4], hash_process[5], hash_process[6]) + hash_process[7] + (K[i] + W[i]) + Sigma1(hash_process[4]);
		hash_process_prime[4] = hash_process[3] + hash_process_prime[0];
		hash_process_prime[5] = hash_process[4];
		hash_process_prime[6] = hash_process[5];
		hash_process_prime[7] = hash_process[6];
		hash_process_prime[0] += Ma(hash_process[0], hash_process[1], hash_process[2]) + Sigma0(hash_process[0]);
		
		// invert a..h & a'..h' instead of copying prime to main, trust the process
		void *tmp = hash_process;
		hash_process = hash_process_prime;
		hash_process_prime = tmp;
		/// or maybe write this below to prevent using tmp
		///// hash_process = hash_process_arr + ((i + 1) % 2) * 8;
		///// hash_process' = hash_process_arr + (i % 2) * 8;
	}
	for (int i = 0; i < 8; i++)
		hash[i] += hash_process[i];
}

static void sha256_hash_constant_filler(uint32_t *hash) {
	hash[0] = 0x6a09e667;
	hash[1] = 0xbb67ae85;
	hash[2] = 0x3c6ef372;
	hash[3] = 0xa54ff53a;
	hash[4] = 0x510e527f;
	hash[5] = 0x9b05688c;
	hash[6] = 0x1f83d9ab;
	hash[7] = 0x5be0cd19;
}

uint32_t	*ft_ssl_sha256(uint8_t *msg, uint64_t len)
{
	return ssl_32(msg, len, &digest_loop, 8, &sha256_hash_constant_filler, 1);
}

uint32_t	*ft_ssl_sha256_fd(int fd, int print_fd)
{
	return ssl_32_fd(fd, print_fd, &digest_loop, 8, &sha256_hash_constant_filler, 1);
}
