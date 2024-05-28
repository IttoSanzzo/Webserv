/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wb_inet_addr.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcosv2 <marcosv2@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 04:21:59 by marcosv2          #+#    #+#             */
/*   Updated: 2024/05/28 04:34:32 by marcosv2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static bool	is_valid_digit(const char *str) {
	while (*str) {
        if (*str < '0' || *str > '9')
            return (false);
        str++;
    }
    return (true);
}

in_addr_t	ws_inet_addr(const char *cp) {
	unsigned char	bytes[4];
	const char*		ptr = cp;
	int				i = 0;

	while (*ptr) {
		if (i >= 4)
			return (INADDR_NONE);
		const char *end = std::strchr(ptr, '.');
		if (!end)
			end = ptr + std::strlen(ptr);
		char	num_str[4] = {0};
		std::strncpy(num_str, ptr, end - ptr);
		if (!is_valid_digit(num_str))
			return (INADDR_NONE);
		int num = std::atoi(num_str);
		if (num < 0 || num > 255)
			return (INADDR_NONE);
		bytes[i++] = (unsigned char)num;
		ptr = (*end) ? end + 1 : end;
	}
	if (i != 4)
		return (INADDR_NONE);
	in_addr_t	addr;
	std::memcpy(&addr, bytes, sizeof(addr));
	return (addr);
}
