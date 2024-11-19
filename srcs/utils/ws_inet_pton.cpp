#include "webserv.hpp"

static bool	is_valid_digit(const char *str) {
	while (*str) {
        if (*str < '0' || *str > '9')
            return (false);
        str++;
    }
    return (true);
}

static int	hex_to_int(char c) {
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

int	ws_inet_pton(int af, const char *src, void *dst) {
	if (af == AF_INET) {
		struct in_addr*	addr = (struct in_addr *)dst;
		unsigned char	bytes[4];
		const char*		ptr = src;
		int				i = 0;
		while (*ptr) {
			if (i >= 4)
				return (0);
			const char*	end = std::strchr(ptr, '.');
			if (!end)
				end = ptr + std::strlen(ptr);
			char	num_str[4] = {0};
			std::strncpy(num_str, ptr, end - ptr);
			if (!is_valid_digit(num_str))
				return (0);
			int num = std::atoi(num_str);
			if (num < 0 || num > 255)
				return (0);
			bytes[i++] = (unsigned char)num;
			ptr = (*end) ? end + 1 : end;
		}
		if (i != 4)
			return (0);
		std::memcpy(&(addr->s_addr), bytes, 4);
		return (1);
	} else if (af == AF_INET6) {
		struct in6_addr*	addr6 = (struct in6_addr *)dst;
		unsigned char		bytes[16] = {0};
		const char*			ptr = src;
		unsigned char*		bptr = bytes;
		int					double_colon = -1;
		while (*ptr) {
			if (bptr - bytes >= 16)
				return (0);
			if (*ptr == ':') {
				if (*(ptr + 1) == ':') {
					if (double_colon != -1)
						return (0);
					double_colon = bptr - bytes;
					ptr += 2;
					continue ;
				}
				ptr++;
			}
			unsigned char	value = 0;
			int				count = 0;
			while (*ptr && *ptr != ':' && count < 4) {
				int hex_val = hex_to_int(*ptr++);
				if (hex_val == -1)
					return (0);
				value = (value << 4) | hex_val;
				count++;
			}
			if (count == 0 || bptr - bytes + 2 > 16)
				return (0);
			*bptr++ = value >> 8;
			*bptr++ = value & 0xff;
			if (*ptr == '\0')
				break;
		}
		if (double_colon != -1) {
			int shift = 16 - (bptr - bytes);
			std::memmove(bytes + double_colon + shift, bytes + double_colon, bptr - bytes - double_colon);
			std::memset(bytes + double_colon, 0, shift);
		}
		std::memcpy(addr6->s6_addr, bytes, 16);
		return (1);
		} else
			return (-1);
}
