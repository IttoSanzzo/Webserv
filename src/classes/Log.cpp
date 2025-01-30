#include "webserv.hpp"

std::time_t Log::startTime = std::time(0);
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
	if (Log::key_log) {
		Log::timestamp();
		std::cout << LOGCOLOR_LOG << message << "\033[0m" << std::endl;
	}
}
void	Log::logNoEndl(const std::string& message) {
	if (Log::key_log) {
		Log::timestamp();
		std::cout << LOGCOLOR_LOG << message << "\033[0m";
	}
}
void	Log::logNoTimestamp(const std::string& message) {
	if (Log::key_log)
		std::cout << LOGCOLOR_LOG << message << "\033[0m" << std::endl;
}
void	Log::info(const std::string& message) {
	if (Log::key_info) {
		Log::timestamp();
		std::cout << LOGCOLOR_INFO << message << "\033[0m" << std::endl;
	}
}
void	Log::warning(const std::string& message) {
	if (Log::key_warning) {
		Log::timestamp();
		std::cout << LOGCOLOR_WARNING << message << "\033[0m" << std::endl;
	}
}
void	Log::error(const std::string& message) {
	if (Log::key_error) {
		Log::timestamp();
		std::cout << LOGCOLOR_ERROR << message << "\033[0m" << std::endl;
	}
}
void	Log::error(const std::exception& ex) {
	Log::timestamp();
	Log::error(ex.what());
}
void	Log::debug(const std::string& message) {
	if (Log::key_debug) {
		Log::timestamp();
		std::cout << LOGCOLOR_DEBUG << message << "\033[0m" << std::endl;
	}
}
void	Log::debugNoEndl(const std::string& message) {
	if (Log::key_debug) {
		Log::timestamp();
		std::cout << LOGCOLOR_DEBUG << message << "\033[0m";
	}
}
void	Log::debugNoTimestamp(const std::string& message) {
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
void	Log::timestamp(void) {
	if (Log::key_timestamp)
		std::cout << LOGCOLOR_TIMESTAMP << "[" << std::time(0) - Log::startTime << "]\033[0m ";
}