#include "webserv.hpp"

bool	Log::getLogState(void) {
	return (Log::key_log);
}
bool	Log::getInfoState(void) {
	return (Log::key_info);
}
bool	Log::getWarningState(void) {
	return (Log::key_warning);
}
bool	Log::getErrorState(void) {
	return (Log::key_error);
}
bool	Log::getDebugState(void) {
	return (Log::key_debug);
}

void	Log::log(const std::string& message) {
	if (Log::key_log)
		std::cout << LOGCOLOR_LOG << message << "\033[0m" << std::endl;
}
void	Log::info(const std::string& message) {
	if (Log::key_info)
		std::cout << LOGCOLOR_INFO << message << "\033[0m" << std::endl;
}
void	Log::warning(const std::string& message) {
	if (Log::key_warning)
		std::cout << LOGCOLOR_WARNING << message << "\033[0m" << std::endl;
}
void	Log::error(const std::string& message) {
	if (Log::key_error)
		std::cout << LOGCOLOR_ERROR << message << "\033[0m" << std::endl;
}
void	Log::error(const std::exception& ex) {
	Log::error(ex.what());
}
void	Log::debug(const std::string& message) {
	if (Log::key_debug)
		std::cout << LOGCOLOR_DEBUG << message << "\033[0m" << std::endl;
}
void	Log::omni(const LogType& type, const std::string& message) {
	switch (type) {
		case (0):
			Log::log(message);
		break;
		case (Info):
			Log::info(message);
		break;
		case (Warning):
			Log::warning(message);
		break;
		case (Error):
			Log::error(message);
		break;
		case (Debug):
			Log::debug(message);
		break;
	}
}