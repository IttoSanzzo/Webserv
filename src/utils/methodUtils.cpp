#include "webserv.hpp"

t_method	methodFromString(const std::string& method) {
	if (method == "GET")
		return (GET);
	else if (method == "POST")
		return (POST);
	else if (method == "PUT")
		return (PUT);
	else if (method == "PATCH")
		return (PATCH);
	else if (method == "DELETE")
		return (DELETE);
	else if (method == "HEAD")
		return (HEAD);
	else if (method == "OPTIONS")
		return (OPTIONS);
	else
		return (no_method);
}
std::string	methodToString(const t_method& method) {
	switch (method) {
		case (GET):
			return ("GET");
		break;
		case (POST):
			return ("POST");
		break;
		case (PUT):
			return ("PUT");
		break;
		case (PATCH):
			return ("PATCH");
		break;
		case (DELETE):
			return ("DELETE");
		break;
		case (HEAD):
			return ("HEAD");
		break;
		case (OPTIONS):
			return ("OPTIONS");
		break;
		default:
			return ("");
		break;
	}
}