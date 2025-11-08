/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 21:58:44 by psalame           #+#    #+#             */
/*   Updated: 2025/11/08 11:48:16 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include "./algorithm/ssl_algorithm.h"

enum flags {
	FLAG_ECHO		=	1 << 0, // -p
	FLAG_QUIET		=	1 << 1, // -q
	FLAG_REVERSE	=	1 << 3, // -r
	FLAG_SUM		=	1 << 4, // -s
	
	// ...
	
	UNKNOWN_FLAG	=	1 << 5
};

int	get_flags(int ac, char **av, int *optind);


#endif
