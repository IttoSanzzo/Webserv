#include "webserv.hpp"

std::string	httpStatusCodeString(const int& code) {
	switch (code) {
		case (200):
			return ("OK");
		break;
		case (201):
			return ("Created");
		break;
		case (204):
			return ("No Content");
		break;
		case (301):
			return ("Moved Permanently");
		break;
		case (302):
			return ("Found");
		break;
		case (304):
			return ("Not Modified");
		break;
		case (400):
			return ("Bad Request");
		break;
		case (401):
			return ("Unautorized");
		break;
		case (403):
			return ("Forbidden");
		break;
		case (404):
			return ("Not Found");
		break;
		case (500):
			return ("Internal Server Error");
		break;
		case (502):
			return ("Bad Gateway");
		break;
		case (503):
			return ("Service Unavailable");
		break;
		default:
			return ("Unknown");
		break;
	}
}