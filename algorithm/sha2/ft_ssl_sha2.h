/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_sha2.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:19:13 by psalame           #+#    #+#             */
/*   Updated: 2025/11/08 15:01:03 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_SHA2_H
# define FT_SSL_SHA2_H
# include <stdint.h>
	
uint32_t	*ft_ssl_sha256(uint8_t *msg, uint64_t len);
uint32_t	*ft_ssl_sha256_fd(int fd, int print_fd);

#endif