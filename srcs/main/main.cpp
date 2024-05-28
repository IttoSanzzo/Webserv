/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcosv2 <marcosv2@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 21:34:40 by marcosv2          #+#    #+#             */
/*   Updated: 2024/05/28 05:29:19 by marcosv2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int	ws_testing(ServerConfig& conf) {
	/* Set */
	try {
		conf.setPort("65500;");
		// conf.setHost("localhost;");
		// conf.setServerName("Bananada de Banana;");
		// conf.setIndex(";");
		// conf.setAutoindex("on;");
		// conf.setClientMaxBodySize("100;");
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}

	/* View */
	std::cout << "Port.......: " << conf.getPort() << std::endl;
	std::cout << "Host.......: " << conf.getHost() << std::endl;
	std::cout << "Name.......: " << conf.getServerName() << std::endl;
	std::cout << "Root.......: " << conf.getRoot() << std::endl;
	std::cout << "Index......: " << conf.getIndex() << std::endl;
	std::cout << "Autoindex..: " << conf.getAutoindex() << std::endl;
	std::cout << "FD.........: " << conf.getFd() << std::endl;
	std::cout << "CliMax.....: " << conf.getClientMaxBodySize() << std::endl;
	return (0);
}


int	main(int ac, char** av) {
	if (ac > 2)
		return (1);

	(void)av;
	ServerConfig	conf;
	return (ws_testing(conf));

	return (0);
}
