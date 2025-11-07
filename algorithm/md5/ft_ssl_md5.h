/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:19:13 by psalame           #+#    #+#             */
/*   Updated: 2025/11/07 21:01:08 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_MD5_H
# define FT_SSL_MD5_H
# include <stdint.h>

uint32_t	*ft_ssl_md5(uint8_t *msg, uint64_t len);
uint32_t	*ft_ssl_md5_fd(int fd);

#endif