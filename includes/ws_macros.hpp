/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws_macros.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcosv2 <marcosv2@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 01:37:43 by marcosv2          #+#    #+#             */
/*   Updated: 2024/05/28 05:23:08 by marcosv2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WS_MACROS_HPP
# define WS_MACROS_HPP

/* Macros */
# define MAX_CLI_SIZE	30000000
# define MAX_URL_SIZE	4096
# define MAX_PORT_NUM	65500

/* Errors */
# define ERR_PORT		"Wrong syntax: port"
# define ERR_HOST		"Wrong syntax: host"
# define ERR_AUTOINDEX	"Wrong syntax: autoindex"
# define ERR_MAXBODY	"Wrong syntax: client_max_body_size"
# define ERR_EXCE		"\033[38;5;196mSERVER CONFIG ERROR : \033[0m"

#endif
